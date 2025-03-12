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
  bottlenecks.
- Limited hardware acceleration: Current systems primarily rely on CPU
  multithreading, with limited optimization for GPU acceleration or SIMD
  enhancements.

Prospective Users
============

CFLib can be applied to the following scenarios:

- Recommendation system development
- Handling High-dimensional sparse data, such as social network analysis

System Architecture
============

Input/Output

- Input: Sparse interaction matrix (e.g., SciPy CSR/CSC format)
- Output: Predicted scores and recommended items, presented as NumPy arrays

Configuration Options

- Computation Backend: General CPU, SIMD, GPU (CUDA)
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

    import cflib import numpy as np

    # Load user-item interaction matrix (sparse matrix) interaction_matrix =
    np.random.randint(0, 2, size=(1000, 1000))

    # Initialize user-user collaborative filtering cf = cflib.UserUserCF()

    # Fit the model cf.fit(interaction_matrix)

    # Generate recommendations for a user user_id = 0 recommendations =
    cf.recommend(user_id, k=10)

    # Evaluate model performance precision = cf.evaluate_precision(test_data)
    recall = cf.evaluate_recall(test_data)

    print(f"Precision: {precision}, Recall: {recall}")


Engineering Infrastructure
============

1.  Automatic Build System:
    
    - Use CMake for building the C++ components.

    - GitHub Actions for continuous integration and automated testing.


2.  Version Control:

    - Git for version control, with a branching strategy for feature
      development and testing.

3.  Testing Framework:
    
    - Google Test for C++ unit tests.

    - pytest for Python API tests.

4.  Documentation:
    
    - Comprehensive documentation using Sphinx or MkDocs.

    - API documentation with examples and usage guidelines.

Schedule
============

Planning phase (03/03-03/17): Research collaborative filtering theories,
existing APIs, and relevant literature.

Week 1 (03/24): Design data preprocessing draft initial documentation and API
specifications.

Week 2 (03/31): Implement fundamental matrix factorization algorithms (ALS,
SGD). (1/3)

Week 3 (04/07): Implement fundamental matrix factorization algorithms (ALS,
SGD). (2/3)

Week 4 (04/14): Implement fundamental matrix factorization algorithms (ALS,
SGD). (3/3); Optimize computation cores using OpenMP and SIMD. (1/3)

Week 5 (04/21): Optimize computation cores using OpenMP and SIMD. (2/3)

Week 6 (04/28): Optimize computation cores using OpenMP and SIMD. (3/3);
Compare performance with existing APIs.

Week 7 (05/05): Test GPU acceleration solutions and evaluate performance
differences; Conduct real-world dataset testing. (1/2)

Week 8 (05/12): Test GPU acceleration solutions and evaluate performance
differences; Conduct real-world dataset testing. (2/2)

Week 9 (05/19): Finalize module integration, complete API documentation and use
cases. (1/2)

Week 10 (05/26): Finalize module integration, complete API documentation and
use cases. (2/2)

Week 11 (06/02): Project presentation

References
============

1. Collaborative Filtering:
   https://en.wikipedia.org/wiki/Collaborative_filtering
2. Koren, Y., Bell, R., & Volinsky, C. (2009). Matrix factorization techniques
   for recommender systems. Computer, 42(8), 30-37.
3. scikit-surprise Documentation: https://surprise.readthedocs.io/en/stable/
