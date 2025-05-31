ZenANN: A High-Performance Vector Similarity Search Library for Python Users
===============================================================================

GitHub: https://github.com/Seco1024/ZenANN.git

Basic Information
-------------------

**ZenANN** is a high-performance approximate nearest neighbor (ANN) similarity
search library designed to be user-friendly for Python developers. It provides
multiple indexing methods, such as **IVF** (Inverted File Index), **HNSW**
(Hierarchical Navigable Small World), and **hybrid-index structures** to
balance between accuracy and speed. The computation kernel of ZenANN will be
optimized for cache efficiency, SIMD acceleration, and algorithm enhancements
beyond existing in-memory libraries.

Problem to Solve
----------------

Similarity search is a fundamental problem in many domains, including
information retrieval, natural language processing, and so on. The key
challenge is to efficiently find the nearest neighbors of a query vector in
high-dimensional space. However, as the data size and dimensionality grow, the
performance of traditional brute-force search (e.g., KD-tree) may suffer from
the **Curse of Dimensionality**.

To solve this problem, **approximate nearest neighbor (ANN)** search aims to
retrieve near-optimal results while significantly reducing computation time. It
trades off a small loss in accuracy for significant speed improvements, making
it ideal for high-dimensional vector search applications.

Although existing in-memory implementations (e.g., Faiss) are highly optimized,
there are still areas for improvement:

- Improved index data layout for better cache locality
- SIMD acceleration for specific algorithms
- Enhancements on data structures and algorithms to better match hardware
  characteristics

Prospective Users
-----------------

ZenANN is designed for developers and researchers working on large-scale
similarity search applications, including:

- **Machine learning engineers** who use ANN search for embedding-based
  retrieval in NLP, computer vision, and recommendation systems.
- **Software developers** who build applications requiring fast vector search
  with a clear, user-friendly programming interface.
- **Data scientists** who perform large-scale similarity analysis on
  high-dimensional datasets.

System Architecture
-------------------

ZenANN will be implemented in C++ for high performance and exposes an intuitive
Python API using pybind11.

Index Hierarchy
~~~~~~~~~~~~~~~

There will be an abstract base index, which provides a unified interface for
different index classes.

1. **Base Index Class**

   - ``indexBase``: Defines the common API for all indexing methods (e.g.,
     ``add()``, ``search()``, ``write_index()``, etc.)

2. **KD-tree Index Class**

   - ``KDTreeIndex``: Serves as a baseline which performs exact search

3. **IVF Index Class**

   - ``IVFIndex``: A cluster-based structure for large dataset

4. **HNSW Index Class**

   - ``HNSWIndex``: A graph-based structure for accurate and efficient ANN

   *Note:* The actual implementation detail of HNSW may be built on Faiss’s
   interface according to development progress.

Processing Flow
~~~~~~~~~~~~~~~

1. Initialize an index (e.g., ``IVFIndex``, ``HNSWIndex``).
2. Build an index with ``add()`` :
   
   - Add the given vector data to a specific index instance.
   - Train index with ``train()`` if needed.
   - Optimize the index data layout to improve cache locality if needed.

3. Perform a query on the specified index instance using ``search()``.
4. Get result set with top-k id & estimated distance for each query.

API Description
---------------

A simple Python example to understand the API design:

.. code:: python

    import zenann

    # Initialize an HNSW index
    index = zenann.HNSWIndex(dim=128, M=16, efConstruction=200)

    # Add vectors to the index and conduct reordering
    index.add(data_vectors)

    # Perform a search
    results = index.search(query_vector, k=5, efSearch=100)

Engineering Infrastructure
--------------------------

Automatic Build System
~~~~~~~~~~~~~~~~~~~~~~

- GNU Make

Version Control
~~~~~~~~~~~~~~~

- Git
- GitHub

Testing Framework
~~~~~~~~~~~~~~~~~

- **Python**: pytest

Documentation
~~~~~~~~~~~~~

- Markdown
- Mermaid

Continuous Integration
~~~~~~~~~~~~~~~~~~~~~~

- GitHub Actions

Schedule
--------

- **Week 1 (3/10):**
  - Set up project repository and initial documentation.
  - Study the relevant field knowledge and adjust the existing interface 
  accordingly.

- **Week 2 (3/17):**
  - Implement the base index class (``indexBase``) with a common interface.
  - Design unit tests for basic index functionalities.
  - Develop pybind11 wrappers.

- **Week 3 (3/24):**
  - Implement KD-tree-based search method 
  - Conduct preliminary benchmarks.
  - Develop pybind11 wrappers.

- **Week 4 (3/31):**
  - Implement HNSW index (``indexHNSW``) with graph-based search.
  - Design unit tests for derived index functionalities.

- **Week 5 (4/7):**
  - Implement IVF index (``indexIVF``) and clustering mechanism.

- **Week 6 (4/14):**
  - Implement ``reorder_layout()`` function to optimize memory locality.
  - Profile cache efficiency and fine-tune memory layout strategies.

- **Week 7 (4/21):**
  - Develop hybrid index (``indexIVF_HNSW``, ``indexHNSW_IVF``).

- **Week 8 (4/28):**
  - Experiment with AVX512 SIMD optimizations on search algorithms.

- **Week 9 (5/5):**
  - Conduct real-world testing on large datasets.

- **Week 10 (5/12):**
  - Implement comprehensive benchmarking tools.
  - Fine-tune algorithm optimizations based on test results.

- **Week 11 (5/19):**
  - Flexible time slot

- **Week 12 (5/26):**
  - Final optimizations and documentation updates.
  - Prepare for final project presentation.

