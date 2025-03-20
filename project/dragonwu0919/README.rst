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

- Stock market
  - market clearing
  - dividend
  - price
- agent
  - predictor
    - condition part
    - forecaste part
- Genetic algorithm

```
function step(){
  # create the dividend for this step

  # for each agent: 
    # filter what predictors can be used in this round
      # for each predictors selected, reflect its accuracy based on price 
data. 
      # among all updated predictors, choose one with best accuracy to use in
      this step.
    # create its own demand based on the chosen predictors 
  
  # do market clearing, this create the price of stock market in next step

  # for each agent with a probability :
    # do genetic algorithm on currently stored predictors. 
}
```

Each agent maintains multiple rules. Each rule contains parameters used by a 
forecaster to estimate future stock prices, and classifiers to determine the 
specific market conditions under which each rule is applicable. Based on these 
conditions, agents calculate their demand for stocks. The genetic algorithm 
iteratively evolves these rules by selecting and modifying them according to 
their performance.

There are two kind of asset in this model: stock and bank. Both of them grant
agent additional interest at the end of each step. The difference is that
dividend iterates with a random number, as banking provide a risk-free interest
rate.

After every agent provide its demand, the model need to do market clearing. To
acheive this, the agents' demand functions are deliberately designed as linear
functions of the stock price, which enables straightforward market clearing
through adjustments to ensure stock supply and demand equilibrium.

The pseudo-code mentioned above will be implemented in C++ layer, which may
looks like following :

- class agent
  - vector<class predictor> predictors;
    - class predictor{uint16 condition; float forecaster[3]};
      // one forecaster requires three parameters
  - void evaluate() 
    // do genetic algorithm
  - float demand()
  - float* strategy()
    // the function above will be called by clearMarket, and it will provide a
    // list of float of the specific forecaster chosen in this step.
  - void evolve()
- class market
  - float rate, price, dividend;
  - void iterateDividend()
  - void clearMarket()

API Description
===============

- `setup`: Initializes the system.
- `step`: Executes a round of trading.
- `show`: Retreive the current parameters of the system.
- `evolve`: Runs one iteration of the genetic algorithm.
- `modify`: Directly modifies a specific parameter of the system.

Engineering Infrastructure
==========================

The calculation is implemented in C++ and wrapped using pybind11 to expose a 
Python API. The C++ codebase is compiled with CMake. Unit testing is performed 
using Google Test for the C++ components and pytest for the Python interface.

Schedule
========

Week 1-3  (03/17):
- Implement core structures, including market setup, agents, rules, 
classifiers, forecasters, and demand functions (excluding the Genetic 
Algorithm).

Weeks 2-3 (03/24 - 03/31):
- Develop the Python wrapper and corresponding API, including functionality 
for system initialization, executing trading steps, recording system 
parameters, running the genetic algorithm, and modifying system configurations.

Weeks 4-6 (04/07 - 04/21):
- Conduct  testing and debugging of the Python API.
- Conduct  testing of the C++ core components.

Weeks 7-9 (04/28 - 05/12):
- Implement GA algorithm.

Weeks 10-11 (05/19 - 05/26):
- If possible, find and implement acceleration method on current codebase 
methods.
- Prepare for the final presentation.

Week 12 (06/02):
- Final project presentation.

References
==========
- Building the Santa Fe Artificial Stock Market, Blake LeBaron, Brandeis 
University, June 2002.
- https://github.com/felixschmitz/ArtificialStockMarketReproduction


# vim: set ff=unix fenc=utf8 ft=python et sw=4 ts=4 sts=4 tw=79:
