# CS455/EE407 Wireless Sensor Network/DVHop Project. 
Team Will Design a virtual Wireless Sensor Network using NS3. This virtual network will use a DVHOP algorithm to find a theoretical location for each node, that does not have GPS capability.

GOALS
- [x] Get DVhop working with NS3_30.
- [x] Design and build a WSN topology with the DV-Hop code.
- [x] Test, simulate and collect data from the WSN with the DV-Hop code.
- [x] Make nodes die gradually.
- [x] Simulate and collect data from a degraded WSN with the DV-Hop code.
- [x] Work on Documentation and Presentation

# Documentation

### Downloading & Configuring our simulation program
> OS requirements: Ubuntu 20.04 or equivalent 
>
> To run our simulation program, you must have NS3.30.1 installed in your
> home directory. (`/home/[USER]/ns-allinone-3.30.1`)

1. Clone this repository anywhere on your system.
2. Run `make copy configure` to set up NS-3 to run our simulation.
3. Run a test simulation with the command `make run`

### Using our build and simulation tasks
- To compile and run the simulation, run `make sim`.
- To run a simulation and capture+convert output data, run `make fullsim`.
- For other smaller tasks, just run `make` to see a list of possible tasks.

### Changes to the original DV-Hop repository
- Ported to NS3.30.1 and C++17
- Added functionality for trilaterating a node's position
- Added functionality for placing nodes in a 2-dimensional grid
- Added functionality for gradually disabling nodes over time
- Added functionality for removing stale entries from a node's hop table
- Simulation now outputs statistics that can be converted to a CSV
- Created a utility to process simulation output
