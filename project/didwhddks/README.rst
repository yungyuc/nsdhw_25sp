===========================================
Optimized Python Library for LCA Problem Solving
===========================================

Basic Information
=================

The Lowest Common Ancestor (LCA) of two nodes in a tree is the deepest node
that is an ancestor of both. Given a rooted tree, the LCA problem involves
efficiently answering multiple queries of the form:

    What is the LCA of nodes u and v in the given tree?

This problem is fundamental in graph theory and has applications in
**computational biology**, **network routing**, **ontology trees**, and
**hierarchical data structures**.

Problems to Solve
=================

Lack of Built-in LCA Support in C++ and Python
----------------------------------------------

- C++ STL does not provide built-in support for LCA queries.
- Python libraries such as ``networkx`` and ``scipy.sparse.csgraph`` provide
  basic shortest path algorithms, but they are not optimized for fast repeated
  LCA queries.
- No specialized LCA algorithms like Binary Lifting, Euler Tour + RMQ, or
  Tarjan's Offline LCA are implemented natively in standard libraries.

Inefficiencies in Existing Graph Libraries
------------------------------------------

- Libraries like ``networkx`` allow generic shortest path computations, but
  each LCA query might thus take :math:`O(N)` due to BFS or DFS traversal.
- ``scipy.sparse.csgraph`` can compute paths on adjacency matrices, but it
  requires :math:`O(N^2)` space, making it impractical for large trees.
- C++ libraries like Boost Graph Library (BGL) provide some tree-related
  operations, but LCA is not a direct feature.

Prospective Users
=================

- **Computational Biology Researchers**
- **Database Administrators and Query Optimizers**
- **Competitive Programmers**

System Architecture
===================

The system will include multiple C++ implementations for solving the Lowest
Common Ancestor (LCA) problem, such as:

- **Binary Lifting**
- **Tarjan's Offline Algorithm**
- **Euler Tour + Range Minimum Query (RMQ)**
- **Euler Tour + RMQ with sqrt decomposition optimization**

Each implementation offers different time and space complexities, allowing
users to select the most suitable approach based on their specific use case.
Additionally, the C++ implementations will be exposed to Python via
``pybind11``, providing a seamless API for Python users.

API Description
===============

Abstract Base Class (LCA)
-------------------------

**Purpose:** Defines a common interface for all LCA algorithms aforementioned.

**Methods:**

- ``add_edge(int u, int v)``: Add an edge between nodes ``u`` and ``v``.
- ``preprocess()``: Preprocess the tree for efficient LCA queries.
- ``query(int u, int v)``: Return the LCA of nodes ``u`` and ``v``.

Derived Classes
---------------

Each derived class provides a specific LCA solving approach:

- ``class BinaryLiftingLCA``
- ``class TarjanLCA``
- ``class RMQLCA`` (without sqrt decomposition)
- ``class RMQLCA`` (optimized with sqrt decomposition)

Factory Class
-------------

**Purpose:** Allow users to dynamically choose a specific LCA algorithm.

**Method:**

- ``static std::unique_ptr<LCA> create(const std::string& method, int n)``:
  Return an instance of the chosen LCA algorithm.

Schedule
========

- **Week 1 - 3 (03/10 - 03/24):**
    - Set up project repository
    - Study the relevant knowledge about binary lifting, Tarjan's algorithm,
      Euler Tour, Range Minimum Query, and sqrt decomposition.

- **Week 4 (03/31):**
    - Generate test cases for future C++ and Python testing
    - Implement abstract base class and verify correctness

- **Week 5 (04/07):**
    - Implement ``BinaryLiftingLCA`` and test correctness
    - ``preprocess()`` would involve ``dfs()`` for tree traversal

- **Week 6 (04/14):**
    - Implement ``TarjanLCA`` and test correctness

- **Week 7 (04/21):**
    - Implement ``RMQLCA`` without sqrt decomposition optimization
    - ``preprocess()`` would involve ``dfs()`` for constructing Euler Tour
    - Build data structures such as Segment Tree and Sparse Table for
      efficient RMQ on Euler Tour
    - Test correctness

- **Week 8 (04/28):**
    - Extend ``RMQLCA`` with sqrt decomposition optimization
    - Implement Factory class
    - Test correctness

- **Week 9 (05/05):**
    - Bind all C++ implementations to Python APIs using ``pybind11``
    - Implement a Python testing program to validate APIs

- **Week 10 - 12 (05/12 - 05/26):**
    - Prepare for final presentation
    - Ensure everything works smoothly
    - Final code review & optimizations

References
==========

- `<https://en.wikipedia.org/wiki/Lowest_common_ancestor>`_

