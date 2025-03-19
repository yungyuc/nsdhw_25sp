================
CFLib: Collaborative Filtering Library
================
[Github Repo] (https://github.com/ckt77/CFLib)

Basic Information
============

CFLib is a Python library designed to implement and optimize collaborative
filtering algorithms for recommendation systems. 

Collaborative filtering is a widely used technique in recommendation systems,
where predictions about a user's interests are made by collecting preferences
from many users. However, as datasets grow in size and complexity, the
computational cost of these algorithms increases significantly. 

CFLib aims to address this by providing efficient implementations that leverage
parallelism, SIMD instructions, and optimized matrix calculation operations.

Data Structure
============

CFLib uses **Compressed Sparse Row (CSR)** and **Compressed Sparse Column (CSC)** 
formats to store sparse matrices:

.. code-block:: python
    :linenos:
    :emphasize-lines: 3-5

    # Example: Constructing a CSR matrix in SciPy
    from scipy.sparse import csr_matrix
    data = [5, 3, 2]     # Non-zero values (e.g., ratings)
    indices = [0, 2, 1]  # Column indices of non-zero entries
    indptr = [0, 2, 3]   # Row offset pointers (row 0: 0-2, row 1: 2-3)
    csr_matrix = csr_matrix((data, indices, indptr), shape=(2, 3))

- **CSR**: Optimized for row operations (e.g., user similarity)

  - `data`: Stores non-zero values

  - `indices`: Column indices of corresponding values

  - `indptr`: Cumulative count of non-zeros per row
- **CSC**: Column-wise analog for item-based operations

Problem to Solve
============

The user-item interaction matrix is typically a vast and highly sparse matrix
(often with more than 99% zeros). This unique data storage structure makes
conventional matrix operations highly inefficient in terms of both
computational speed and storage space.

Despite the availability of existing APIs such as scikit-surprise, several
challenges remain:

- Efficient sparse matrix operations and storage data structures: Most existing
  APIs rely on SciPy’s sparse matrix operations, which may become performance
  bottlenecks. We will use compressed sparse row (CSR) and compressed sparse
  column (CSC) formats to store the matrix efficiently and perform operations
  faster.
- Limited hardware acceleration: Current systems primarily rely on CPU
  multithreading, with limited optimization for GPU acceleration or SIMD
  enhancements. We will leverage SIMD instructions such as AVX2 and AVX-512 to
  accelerate matrix operations.

Prospective Users
============

CFLib can be applied to the following scenarios:

- Recommendation system development
- Handling High-dimensional sparse data, such as social network analysis

System Architecture
============

Input/Output

- Input: Sparse interaction matrix (e.g., SciPy CSR/CSC format), where the two 
  dimensions of the matrix correspond to users and item options (e.g., for 
  product recommendations in a marketplace, the item options would be products;
  for recommendations on a video streaming site, the item options could be 
  videos). Each (user, item) pair represents the user's rating or interaction 
  count with the item, and if there is no interaction record, the value is 0.

- Output: Predicted scores and recommended items, presented as NumPy arrays

Configuration Options

- Computation Backend: General CPU, SIMD (AVX2, AVX-512), GPU (CUDA)
- Number of Recommended Items: A tunable hyperparameter that determines the
  number of output items

Optimizations

- Parallelism: Utilize multi-threading to speed up computations.
- SIMD: Leverage CPU SIMD instructions for vectorized operations.
- Sparse Matrix Operations: Optimize storage and computations for sparse
  matrices.

API Description
============

CFLib will provide an intuitive Python API for performing collaborative
filtering tasks. Below is an example of how the API might be used:

.. code-block:: python

    import cflib
    import numpy as np

    # Load user-item interaction matrix (sparse matrix)
    interaction_matrix = np.random.randint(0, 2, size=(1000, 1000))

    # Initialize user-user collaborative filtering
    cf = cflib.UserUserCF()

    # Fit the model
    cf.fit(interaction_matrix)

    # Generate recommendations for a user
    user_id = 0
    recommendations = cf.recommend(user_id, k=10)

    # Evaluate model performance
    precision = cf.evaluate_precision(test_data)
    recall = cf.evaluate_recall(test_data)

    print(f"Precision: {precision}, Recall: {recall}")

Engineering Infrastructure
============

1. Automatic Build System:
    - Use CMake for building the C++ components.
    - GitHub Actions for continuous integration and automated testing.

2. Version Control:
    - Git for version control, with a branching strategy for feature
      development and testing.

3. Testing Framework:
    - Google Test for C++ unit tests.
    - pytest for Python API tests.

4. Documentation:
    - Comprehensive documentation using Sphinx or MkDocs.
    - API documentation with examples and usage guidelines.

Schedule
============

Planning phase (03/03-03/17): Research collaborative filtering theories,
existing APIs, and relevant literature.

Week 1 (03/24): Design data preprocessing draft initial documentation and API
specifications.

Week 2 (03/31): Implement fundamental matrix factorization algorithms (ALS,
SGD). (1/3); Write unit tests for ALS and SGD implementations and CSR/CSC 
operations.

Week 3 (04/07): Implement fundamental matrix factorization algorithms (ALS,
SGD). (2/3); Integration tests for similarity computation.

Week 4 (04/14): Implement fundamental matrix factorization algorithms (ALS,
SGD). (3/3); Optimize computation cores using OpenMP and SIMD. (1/3)

Week 5 (04/21): Optimize computation cores using OpenMP and SIMD. (2/3); 
**Test GPU acceleration kernel validation (error tolerance < 1e-5).**;
**Real-world dataset testing (MovieLens 10M); Performance benchmarking. (1/2)**

Week 6 (04/28): Optimize computation cores using OpenMP and SIMD. (3/3); 
**Real-world dataset testing (MovieLens 10M); Performance benchmarking. (2/2)**

Week 7 (05/05): **Refine GPU memory management**; Conduct stress testing 
(1M+ synthetic dataset). (1/2)

Week 8 (05/12): **Finalize GPU optimization**; Analyze benchmark results. (2/2)

Week 9 (05/19): Finalize module integration, complete API documentation and use
cases. (1/2)

Week 10 (05/26): Finalize module integration, complete API documentation and
use cases. (2/2); Perform end-to-end testing.

Week 11 (06/02): Project presentation

References
============

1. Collaborative Filtering:
   https://en.wikipedia.org/wiki/Collaborative_filtering
2. Koren, Y., Bell, R., & Volinsky, C. (2009). Matrix factorization techniques
   for recommender systems. Computer, 42(8), 30-37.
3. scikit-surprise Documentation: https://surprise.readthedocs.io/en/stable/
