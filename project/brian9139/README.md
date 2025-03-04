# Adaptive Routing for Network-on-Chip Hotspot Simulation

## Basic Information

**GitHub:** 

## Problem to Solve

Network-on-Chip (NoC) is a key technology for modern many-core processors
and chiplet-based architectures. Common small-scale NoCs, such as those in
multi-core CPUs or AI accelerators, are mostly 4×4 or 8×8. As for
communication, the network communication between chiplets is the bottleneck
of system communication compared to intra-chiplet communication.
Furthermore, when NoC design is load-imbalanced, where certain regions of
the network become hotspots due to high traffic, these hotspots lead to
increased latency and reduced throughput.

This project focuses on implementing an adaptive routing algorithm in a
software-based NoC simulator to tackle hotspot congestion, thereby
decreasing the load imbalance factor (LBF).

## Prospective Users

- Computer architects studying NoC performance optimization.
- Chip designers looking for efficient routing strategies for
  chiplet-based architectures.
- Researchers developing novel adaptive routing algorithms.
- Students learning about NoC behavior and performance metrics.

## System Architecture

Three formulas for evaluating NoC load imbalance:

1. **Congestion calculation:**
   
   ```math
   C(R) = \frac{\text{buffer maximum capacity}}{\text{number of packets in the buffer}}
   ```
   
   This equation represents the buffer congestion ratio, which measures how
   full a buffer is relative to its maximum capacity.

2. **Adaptive routing decision:**
   
   ```math
   R_{\mathrm{next}} = \arg\min_{R_j \in \mathrm{XY-Neighbors}(R_i)} C(R_j)
   ```
   
   This method compares the congestion levels of the four neighboring nodes
   of the current node and selects the one with the lowest congestion as the
   next hop. This helps avoid entering hotspot areas, reducing the overall
   system burden.

3. **Load balance factor (LBF):**
   
   ```math
   LBF = \frac{\max C(R)}{\frac{1}{N} \sum C(R)}
   ```
   
   The LBF measures how evenly traffic is distributed across the network.
   It is defined as the ratio between the maximum congestion in the network
   and the average congestion across all routers. A higher LBF indicates
   higher load imbalance.

### Implementation Plan

1. Implement an **8×8 2D NoC** in C++, where each node is a router.
2. Initialize each node with **maximum capacity** and **current load**.
3. Simulate **hotspot scenarios** targeting an LBF of approximately 1.5–2.
4. Implement an **adaptive routing function** in C++.
5. Generate **random traffic** at each cycle to simulate real-world
   conditions.
6. Observe changes in LBF and evaluate congestion mitigation.
7. Bind the C++ NoC core to Python using **Pybind11**.
8. Use Python for **initialization and visualization** of the simulation.

## API Description

The system exposes an API for users to configure and run NoC simulations.

### Python Interface:

```python
from noc_sim import NoCSimulator

# Initialize NoC
noc = NoCSimulator(size=8, routing_algorithm="adaptive")

# Set traffic pattern
noc.generate_traffic(mode="hotspot", hotspot_area=[(3,3), (3,4), (4,3), (4,4)])

# Run simulation
noc.run_simulation(num_cycles=1000)

# Evaluate performance
noc.visualize_congestion()  # Displays congestion levels in the 8×8 NoC
noc.visualize_performance_metrics()  # Plots LBF and latency over time
```

## Engineering Infrastructure

- **Programming languages:** Python (simulation, visualization), C++
  (adaptive routing core)
- **Build system:** CMake for C++, Python setuptools for Python wrapper
- **Version control:** Git
- **Testing framework:** Google Test (C++), pytest (Python)

## Schedule

| Week  | Date   | Task                                     |
|-------|--------|-----------------------------------------|
| 1     | 03/24  | Define data structure & setup         |
| 2     | 03/31  | Implement NoC simulation framework    |
| 3     | 04/07  | Implement traffic generation model    |
| 4     | 04/14  | Implement adaptive routing function   |
| 5     | 04/21  | Unit testing for routing algorithm   |
| 6     | 04/28  | Bind C++ NoC core to Python (Pybind11) |
| 7     | 05/05  | Implement visualization module       |
| 8     | 05/12  | Performance evaluation & metrics     |
| 9     | 05/19  | System integration & optimizations   |
| 10    | 05/26  | Prepare presentation                 |
