C-STAM
================

Basic Information
================

C-STAM is an implementation of the Santa Fe Institute Artificial Stock Market 
(SFI-ASM). It is written in C++ for computational efficiency, while Python is 
used to provide a user-friendly API.

GitHub Repository: https://github.com/dragonwu0919/C-STAM

Problem to Solve
================

The Santa Fe Artificial Stock Market (SFI-ASM) is an agent-based computational 
model designed to simulate complex financial market dynamics through 
interactions of adaptive agents. Each agent evolves trading strategies over 
time using a genetic algorithm. To handle the significant computational load 
efficiently, this project implements the SFI-ASM in C++, leveraging its 
computational performance, while providing a convenient Python API for ease of 
use and integration.


Prospective Users
=================

This project is suited for:
- financial economists and researchers who are interested in exploring market 
dynamics through computational simulations.
- people who are interested in agent-based modeling.

System Architecture
===================

The Santa Fe Institute Artificial Stock Market (SFI-ASM) simulates a market 
comprising stocks and a banking asset, in which agents perform stock 
transactions.

- Agent
  - Rule
  - Forecaster
  - Classifier
  - Demand
  - Genetic Algorithm (GA)

Each agent maintains multiple rules. Each rule contains parameters used by a 
forecaster to estimate future stock prices, and classifiers to determine the 
specific market conditions under which each rule is applicable. Based on these 
conditions, agents calculate their demand for stocks. The genetic algorithm 
iteratively evolves these rules by selecting and modifying them according to 
their performance.

- Market
  - Asset
    - Stock
    - Bank
  - Trade

The SFI-ASM explicitly includes a bank asset, providing a risk-free interest 
rate. The agents' demand functions are deliberately designed as linear 
functions of the stock price, which enables straightforward market clearing 
through adjustments to ensure stock supply and demand equilibrium.

API Description
===============

- `setup`: Initializes the system.
- `step`: Executes a round of trading.
- `record`: Records the current parameters of the system.
- `iterate`: Runs one iteration of the genetic algorithm.
- `modify`: Directly modifies a specific parameter of the system.

Engineering Infrastructure
==========================

The calculation is implemented in C++ and wrapped using pybind11 to expose a 
Python API. The C++ codebase is compiled with CMake. Unit testing is performed 
using Google Test for the C++ components and pytest for the Python interface.

Schedule
========

Week 1 (03/17):
- Implement core structures, including market setup, agents, rules, 
classifiers, forecasters, and demand functions (excluding the Genetic 
Algorithm).

- Research and identify potential computational acceleration methods, focusing 
on optimizing agent-based computations, improving market simulation 
efficiency, and reducing execution time in the C++ core.

Weeks 2-3 (03/24 - 03/31):
- Implement the Genetic Algorithm (GA).

Weeks 4-5 (04/07 - 04/14):
- Conduct  testing of the C++ core components.

Weeks 6-7 (04/21 - 04/28):
- Develop the Python wrapper and corresponding API, including functionality 
for system initialization, executing trading steps, recording system 
parameters, running the genetic algorithm, and modifying system configurations.

Weeks 8-9 (05/05 - 05/12):
- Conduct  testing and debugging of the Python API.

Weeks 10-11 (05/19 - 05/26):
- Implement and optimize the previously identified computational acceleration 
methods. 

Week 12 (06/02):
- Final project presentation.

References
==========
- Building the Santa Fe Artificial Stock Market, Blake LeBaron, Brandeis 
University, June 2002.
- https://github.com/felixschmitz/ArtificialStockMarketReproduction


# vim: set ff=unix fenc=utf8 ft=python et sw=4 ts=4 sts=4 tw=79:
