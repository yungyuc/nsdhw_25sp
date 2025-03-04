
Basic Information
=================

GitHub Repository:
  https://github.com/johnny19436/risk-management-engine

About:
  A command-line tool for real-time risk analysis in hedge funds using modern
  C++ (with OpenCL) and Python.

Problem to Solve
================

Hedge funds need efficient and reliable risk assessment tools. This project
addresses the challenge of computing real-time Value-at-Risk (VaR) and Expected
Shortfall (ES) metrics. Key points include:

1. Financial industry risk management.
2. Advanced numerical methods for Monte Carlo VaR.
3. Use of parallel computing (via OpenCL) to accelerate large-scale matrix
   operations and stress testing simulations.

Prospective Users
=================

The primary users of this software are:

- Hedge Fund Managers: to monitor and evaluate portfolio risks in real time.
- Risk Analysts: to perform detailed risk assessments and simulate stress
  scenarios.
- Financial Institutions: to integrate a robust risk management engine into their
  existing systems.

System Architecture
===================

The system is a hybrid application with two main modules:

- C++ Module:
   Implements fast matrix computations using OpenCL.
   Performs risk calculations (VaR, ES) using Monte Carlo method.

- Python Module:
    Acts as a high-level interface and provides data analysis, visualization,
    and scripting capabilities using NumPy, Pandas, and Matplotlib.

Workflow:
  1. The user inputs portfolio data and risk parameters via the command-line.
  2. The Python module processes the input and calls the C++ module for
     computation-intensive tasks.
  3. Results are returned, analyzed, and displayed with visual reports.

API Description
===============

The system offers APIs for both C++ and Python.

C++ API:
  - Provides functions such as `computeVaR(portfolio, params)` and
    `simulateStressTest(portfolio, conditions)`.
  - Designed for high performance and parallel computation.

Python API:
  - Offers wrapper functions for C++ APIs, allowing seamless integration in
    Python scripts.

Engineering Infrastructure
==========================

The project is supported by a robust engineering setup:

1. Automatic Build System:
     Single-command build process using CMake for the C++ module and a
     setup.py script for the Python module.
2. Version Control:
     Code is hosted on GitHub with frequent commits, pull requests, and branch
     management.

3. Testing Framework:

   - C++ Testing: Implements unit tests using Google test for C++. These tests cover the
     critical algorithms (e.g., risk metric computations).
    
   - Python Testing: Uses pytest to validate API wrappers, data processing,
     and visualization modules.

Schedule
========

The project is planned for 8-week development:

**Planning phase** (6 weeks from mm/dd to mm/dd):
  * Week 1: Set up repository, initial project skeleton, and prototype
    for basic functionalities.
    
  * Week 2: Develop core risk computation functions in C++.
  * Week 3: Create Python wrappers and a basic CLI interface.
  * Week 4: Integrate C++ and Python modules; begin unit testing on C++.
  * Week 5: Optimize parallel computations and enhance performance.
  * Week 6: Full system testing and comprehensive documentation.
**Integration** and Presentation Phase (2 weeks):
  * Week 7: Final feature integration, address feedback, polish CLI.
  * Week 8: Prepare final presentation and finalize repository details.

References
==========

- `Value at Risk (VaR) - Investopedia <https://www.investopedia.com/terms/v/var.asp>`__
- `Expected Shortfall (ES or CVaR) - Investopedia <https://www.investopedia.com/terms/c/conditional_value_at_risk.asp>`__
