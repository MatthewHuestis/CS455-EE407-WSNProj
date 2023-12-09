/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DVHOP_H
#define DVHOP_H

#include "ns3/node.h"
#include "ns3/random-variable-stream.h"
#include "ns3/output-stream-wrapper.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/nstime.h"
#include "ns3/timer.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"

#include "distance-table.h"

#include <map>


namespace ns3 {
  namespace dvhop{

    class RoutingProtocol : public Ipv4RoutingProtocol{
    public:
      static const uint32_t DVHOP_PORT;
      static TypeId GetTypeId (void);


      RoutingProtocol();
      virtual ~RoutingProtocol();
      virtual void DoDispose();

      //From Ipv4RoutingProtocol
      Ptr<Ipv4Route>  RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
      bool            RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                                  UnicastForwardCallback   ufcb,
                                  MulticastForwardCallback mfcb,
                                  LocalDeliverCallback     ldcb,
                                  ErrorCallback            errcb);
      virtual void SetIpv4(Ptr<Ipv4> ipv4);
      virtual void NotifyInterfaceUp (uint32_t interface);
      virtual void NotifyInterfaceDown (uint32_t interface);
      virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
      virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
      virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const;
      virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit) const;

      int64_t AssignStreams(int64_t stream);

      // Sets whether this node is a beacon
      void SetIsBeacon(bool isBeacon)    { m_isBeacon = isBeacon; }

      // Sets this node's position
      void SetPosition(double x, double y) { m_xPosition = x; m_yPosition = y; }

      // Sets this node's preset position for error checking
      void SetPresetXY(double x, double y) { m_presetX = x; m_presetY = y; }

      // Gets this node's X position
      double GetXPosition()               { return m_xPosition; }

      // Gets this node's Y position
      double GetYPosition()               { return m_yPosition; }

      // Returns true if this node is a beacon
      bool  IsBeacon()                    { return m_isBeacon; }
      
      // Prints this node's distance table
      void  PrintDistances(Ptr<OutputStreamWrapper> stream, Ptr<Node> node) const;

    private:
      // Start protocol operation
      void        Start    ();

      // Sends a packet to the given destination
      void        SendTo   (Ptr<Socket> socket, Ptr<Packet> packet, Ipv4Address destination);

      // Callback to process a received packet
      void        RecvDvhop(Ptr<Socket> socket);

      // Finds the socket at the given address
      Ptr<Socket> FindSocketWithInterfaceAddress (Ipv4InterfaceAddress iface) const;

      //In case there exists a route to the destination, the packet is forwarded
      bool        Forwarding(Ptr<const Packet> p, const Ipv4Header &header, UnicastForwardCallback ufcb, ErrorCallback errcb);
      void        SendUnicastTo(Ptr<Socket> socket, Ptr<Packet> packet, Ipv4Address destination);

      //HELLO intervals and timers
      Time   HelloInterval;
      Timer  m_htimer;
      void   SendHello();
      void   HelloTimerExpire();

      //Table to store the hopCount to each beacon
      DistanceTable  m_disTable;
      void UpdateHopsTo (Ipv4Address beacon, uint16_t hops, double x, double y);

      // Vector norm for trilateration
      double V_Norm(double x, double y);

      // Trilaterate this node's position from 3 beacons
      std::pair<double, double> Trilaterate(double x_1, double y_1, 
          double hops_1, double x_2, double y_2, double hops_2, double x_3, 
          double y_3, double hops_3);
      
      // Check if a vector contains an index
      bool HasIndex(std::vector<uint>& indices, uint search_index);

      // Gets the average hop size between this node and 3 beacons
      double AvgHopSize(double b1_x, double b1_y, double b2_x,
                      double b2_y, double b3_x, double b3_y, double avg_nhops);

      // Boolean to identify if this node acts as a Beacon
      bool m_isBeacon;

      // This node's position info
      double m_xPosition;
      double m_yPosition;

      // Reference position (used for error checking)
      double m_presetX;
      double m_presetY;

      //IPv4 Protocol
      Ptr<Ipv4>   m_ipv4;

      // Raw socket per each IP interface, map socket -> iface address (IP + mask)
      std::map< Ptr<Socket>, Ipv4InterfaceAddress > m_socketAddresses;

      // DV-hop sequence number
      uint32_t    m_seqNo;

      // Used to simulate jitter
      Ptr<UniformRandomVariable> m_URandom;
    };
  }

}

#endif /* DVHOP_H */

