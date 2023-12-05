/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/dvhop-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/netanim-module.h"
#include "ns3/wifi-mac-helper.h"
#include <iostream>
#include <cmath>

using namespace ns3;

/**
 * \brief Test script.
 *
 * This script creates 1-dimensional grid topology and then ping last node from the first one:
 *
 * [10.0.0.1] <-- step --> [10.0.0.2] <-- step --> [10.0.0.3] <-- step --> [10.0.0.4]
 *
 *
 */
class DVHopExample
{
public:
  DVHopExample ();
  /// Configure script parameters, \return true on successful configuration
  bool Configure (int argc, char **argv);
  /// Run simulation
  void Run ();
  /// Report results
  void Report (std::ostream & os);

  /// Simulate damage to the WSN
  void DamageWSN(int n_to_damage);

  /// Disable a node
  void DisableNode(int index);

private:
  ///\name parameters
  //\{
  /// Number of nodes
  uint32_t size;
  /// Number of nodes which will become anchor nodes, must be at least 1
  uint32_t beacons;
  /// Approx. distance between nodes, meters
  double step;
  /// Simulation time, seconds
  double totalTime;
  /// Write per-device PCAP traces if true
  bool pcap;
  /// Print routes if true
  bool printRoutes;
  /// Number of nodes to damage
  uint32_t d_extent;
  //\}

  ///\name network
  //\{
  NodeContainer nodes;
  NetDeviceContainer devices;
  Ipv4InterfaceContainer interfaces;
  //\}

private:
  void CreateNodes ();
  void CreateDevices ();
  void InstallInternetStack ();
  void InstallApplications ();
  void CreateBeacons();
};

int main (int argc, char **argv)
{
  DVHopExample test;
  if (!test.Configure (argc, argv))
    NS_FATAL_ERROR ("Configuration failed. Aborted.");

  test.Run ();
  test.Report (std::cout);
  return 0;
}

//-----------------------------------------------------------------------------
DVHopExample::DVHopExample () :
  size (50),
  beacons (12),
  step (50),
  totalTime (10),
  pcap (true),
  printRoutes (true),
  d_extent(0)
{
}

//configues settings and defaults of the ns3 simulation
bool DVHopExample::Configure (int argc, char **argv)
{
  // Enable DVHop logs by default. Comment this if too noisy
  LogComponentEnable("DVHopRoutingProtocol", LOG_LEVEL_ALL);

  SeedManager::SetSeed (12345);
  CommandLine cmd;

  cmd.AddValue ("pcap", "Write PCAP traces.", pcap);
  cmd.AddValue ("printRoutes", "Print routing table dumps.", printRoutes);
  cmd.AddValue ("size", "Number of nodes.", size);
  cmd.AddValue ("beacons", "Number of nodes that are beacons, must be at least 1", beacons);
  cmd.AddValue ("time", "Simulation time, s.", totalTime);
  cmd.AddValue ("step", "Grid step, m", step);
  cmd.AddValue ("damageExtent", "How much to damage the WSN", d_extent);

  cmd.Parse (argc, argv);
  return true;
}

//runs ns3 simulation
void DVHopExample::Run ()
{
//  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue (1)); // enable rts cts all the time.
  CreateNodes ();
  CreateDevices ();
  InstallInternetStack ();
  CreateBeacons();
  DamageWSN(d_extent);

  std::cout << "Starting simulation for " << totalTime << " s ...\n";

  Simulator::Stop (Seconds (totalTime));

  AnimationInterface anim("animation.xml");

  Simulator::Run ();
  Simulator::Destroy ();
}

void DVHopExample::Report (std::ostream &)
{
}

//Disables the node at specified index
void DVHopExample::DisableNode(int index)
{
  std::cout << "@STATS@TIME@" << Simulator::Now().GetMilliSeconds();
  std::cout << "@EVENT@DISABLED_NODE@\n";

  Ptr<ConstantPositionMobilityModel> mob = nodes.Get(index)->GetObject<ConstantPositionMobilityModel>();
  mob->SetPosition(Vector(100000 * (index + 1), 100000 * (index + 1), 100000 * (index + 1)));
}

//takes the number of nodes to damage then figures out a list of random nodes to disable and then calls DisableNode to disable them
void DVHopExample::DamageWSN(int n_to_damage) {
  std::cout << "Damaging " << n_to_damage << " nodes.\n";
  for(int i = 0; i < n_to_damage; i++) {
    int r_index = std::rand() % (size - 1);
    //this->DisableNode(r_index);
    int total_time_ms = (int) totalTime * 1000;
    int scheduled_time_ms = std::rand() % total_time_ms;
    std::cout << "Scheduled damage at " << scheduled_time_ms << "\n";
    Simulator::Schedule(MilliSeconds(scheduled_time_ms), &DVHopExample::DisableNode, this, r_index);
  }
}

//creates size nodes (size is set in line 90)
void DVHopExample::CreateNodes ()
{
  std::cout << "Creating " << (unsigned)size << " nodes " << step << " m apart.\n";
  nodes.Create (size);
  // Name nodes
  for (uint32_t i = 0; i < size; ++i)
    {
      std::ostringstream os;
      os << "node-" << i;
      std::cout << "Creating node: "<< os.str ()<< std::endl ;
      Names::Add (os.str (), nodes.Get (i));
    }
  // Create static grid
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (step),
                                 "MinY", DoubleValue (step),
                                 "DeltaX", DoubleValue (step),
                                 "DeltaY", DoubleValue (step),
                                 "GridWidth", UintegerValue ((uint) sqrt(size)),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
}

//create becon nodes
void DVHopExample::CreateBeacons ()
{
  if (beacons <= 0) {
    std::cout << "\"beacons\" was set to " << beacons << ", corrected to 1 to avoid issue.";
    beacons = 1;
  }
  if (beacons >= size) {
    std::cout << "\"beacons\" was set to " << beacons << ", corrected to " << (size - 1) << " to avoid issue.";
    this->beacons = this->size - 1;
  }

uint32_t stepThrough = this->size / this->beacons;

  Ptr<Ipv4RoutingProtocol> proto;
  Ptr<dvhop::RoutingProtocol> dvhop;
  Ptr<ConstantPositionMobilityModel> mob;
  for(uint32_t i = 0; i < size; i++) {
    proto = nodes.Get (i)->GetObject<Ipv4>()->GetRoutingProtocol ();
    mob = nodes.Get(i)->GetObject<ConstantPositionMobilityModel>();
    dvhop = DynamicCast<dvhop::RoutingProtocol> (proto);
    double x_offset = ((double) (std::rand() % 1000)) / 100.0;
    double y_offset = ((double) (std::rand() % 1000)) / 100.0;
    double r_x = mob->GetPosition().x + x_offset;
    double r_y = mob->GetPosition().y + y_offset;
    dvhop->SetPosition(r_x, r_y);
  }

  for ( uint32_t i = 0; i < beacons; i++) {
    proto = nodes.Get (i * stepThrough)->GetObject<Ipv4>()->GetRoutingProtocol ();
    dvhop = DynamicCast<dvhop::RoutingProtocol> (proto);
    dvhop->SetIsBeacon (true);
  }
}


void DVHopExample::CreateDevices ()
{
  WifiMacHelper wifiMac = WifiMacHelper ();
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  WifiHelper wifi = WifiHelper();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"), "RtsCtsThreshold", UintegerValue (0));
  devices = wifi.Install (wifiPhy, wifiMac, nodes);

  if (pcap)
    {
      wifiPhy.EnablePcapAll (std::string ("aodv"));
    }
}

void DVHopExample::InstallInternetStack ()
{
  DVHopHelper dvhop;
  // you can configure DVhop attributes here using aodv.Set(name, value)
  InternetStackHelper stack;
  stack.SetRoutingHelper (dvhop); // has effect on the next Install ()
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.0.0.0");
  interfaces = address.Assign (devices);

  Ptr<OutputStreamWrapper> distStream = Create<OutputStreamWrapper>("dvhop.distances", std::ios::out);
  dvhop.PrintDistanceTableAllAt(Seconds(9), distStream);

  if (printRoutes)
    {
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("dvhop.routes", std::ios::out);
      dvhop.PrintRoutingTableAllAt (Seconds (8), routingStream);
    }
}
