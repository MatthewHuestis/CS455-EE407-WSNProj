# CS455/EE407 Wireless Sensor Network/DVHop Project.
### Project description:
Teams will design a virtual Wireless Sensor Network using NS3. This virtual network will use the DV-Hop algorithm to estimate a location for each node that does not have GPS capability.

GOALS
- [x] Get DVhop working with NS3_30.
- [x] Design and build a WSN topology with the DV-Hop code.
- [x] Test, simulate and collect data from the WSN with the DV-Hop code.
- [x] Make nodes die gradually.
- [x] Simulate and collect data from a degraded WSN with the DV-Hop code.
- [x] Analyze and present data collected from both simulations
- [ ] Create documentation for our modifications

# Documentation

### (1) Downloading & configuring our simulation program
> OS requirements: Ubuntu 20.04 or equivalent 
>
> To run our simulation program, you must have NS3.30.1 installed in your
> home directory. (`/home/[USER]/ns-allinone-3.30.1`)

1. Clone this repository anywhere on your system.
2. Open a terminal in the repository directory and run `make copy configure` to
copy our simulation files to the installed NS3's sources and link the required
modules.
3. Run a test simulation with the command `make run`.

### (2) Using our build and simulation tasks
- To compile and run the simulation, run `make sim`.
- To run a simulation and capture+convert its output data, run `make fullsim`.
- For other smaller tasks, just run `make` to see a list of possible tasks.

### (3) Running the simulation manually
Running the simulation manually allows you finer control over its parameters.
Make sure you are in your NS3 directory and run `./waf --run dvhop-example`.
You can use the following arguments to customize the simulation:
 - `pcap` (bool): Whether to output PCAP files
 - `printRoutes` (bool): Whether to print routes
 - `size` (uint): Total number of nodes to simulate
 - `beacons` (uint): Number of anchor nodes to simulate. Must be less than `size`
 - `time` (uint): Simulation time (seconds)
 - `damageExtent` (uint): Number of times a node will be selected and disabled 
 to simulate critical conditions

### (4) Changes to the original DV-Hop repository
This repository is modified from <https://github.com/pixki/dvhop>.
We included
- Ported to NS3.30.1 and C++17
- Added functionality for trilaterating a node's position
- Added functionality for placing nodes in a 2-dimensional grid
- Added functionality for gradually disabling nodes over time
- Added functionality for removing stale entries from a node's hop table
- Simulation now outputs statistics that can be converted to a CSV
- Created a utility to process simulation output

### (5) Simulation output processor utility
Bundled with the DV-Hop simulation is the source code and executable for a small
utility that converts the simulation output to a CSV file. It can be built using
the Makefile in its directory (`stats_to_csv`). The tasks in the project's
Makefile will automatically use the utility to parse the simulation output, but
you can also use it yourself (it just reads from `stdin` and writes to `stdout`).
