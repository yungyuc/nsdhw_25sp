LUT-based Mixed-Precision GEMM
==============================


Basic Information
=================

- **Project**: mpGEMM
- **Github**: https://github.com/5000user5000/mpGEMM

On resource-constrained devices, such as embedded systems, running deep 
learning models, especially LLMs, is highly computationally expensive. Low-bit 
quantization is a popular solution to reduce memory consumption. However, when 
weights are quantized below 8 bits, performing matrix multiplication between 
the weight matrix and an FP16 activation matrix requires dequantization to a 
common precision, as hardware lacks native support for mixed-precision matrix 
multiplication (mpGEMM). Some recent research suggests using lookup tables 
(LUTs) to replace dequantization, further reducing computational overhead.

.. image:: ./img/lut.png


Problem to Solve
================

- Support mixed-precision matrix computation (weight INT1~4) where activation
tensors are limited to FP16.

- Implement **precomputed lookup table (LUT)-based computation** to accelerate
low-bit matrix multiplications.

- Enable the LUT to reside in the fastest on-chip memory and parallel lookup.

- Preliminary estimates suggest that vendor libraries such as MKL or
Accelerate can be **10× to 1000×** faster than naive GEMM implementations.
This project aims to evaluate how closely LUT-based methods can approach these
performance levels in mixed-precision low-bit GEMM.

Prospective Users
=================

This project will benefit:

1. **Embedded AI Developers**: Those optimizing AI models for deployment on
edge devices.

2. **Data Scientists**: Users who need optimized inference on diverse hardware
setups.

3. **Academia & Research Labs**: Those investigating numerical optimization
and quantization techniques.


System Architecture
===================

The project consists of the following components:

**Lookup Table Component**

- Precomputes and stores the LUT in the on-chip memory.

- Uses SIMD instructions (intel AVX) to accelerate lookup.

**GEMM Component**

- Implements the  matrix multiplication algorithm in C++.

- With 3 modes: **LUT-based**, **naive**, and **vendor** libraries
(MKL/Accelerate).

- Supports mixed-precision matrix computation (weight INT1~4, activation FP16).

**Benchmarking Component**

- The benchmarking Component will compare latency between LUT-based GEMM, vendor
libraries, and traditional dequantization-based methods.



*****************
 API Description
*****************

.. code-block:: python

  import mpGEMM

  # Initialize GEMM engine (Choose backend: "lut", "mkl", "naive")
  gemm = mpGEMM(backend="lut", use_simd=True)

  # Perform GEMM operation, specifying weight bit-width
  output = gemm.matmul(weights, activations, weight_bit_width=4)

  # Benchmark different computation modes
  gemm.benchmark(methods=["lut", "mkl", "naive"], num_runs=10)


Engineering Infrastructure
==========================

- **Automated Build System:** Uses CMake to set up the C++ build system and
setuptools to build Python packages.

- **CI**: GitHub Actions for automated testing and benchmarking. The CI
pipeline includes:

  - **Correctness tests**: Ensures matrix multiplication results are
numerically accurate.

  - **Performance benchmarks**: Compares LUT-based GEMM with traditional
dequantization-based methods and vendor libraries (MKL).

- **Version Control:** Uses Git for version management, with all development
processes submitted to the GitHub repository.


Schedule
========

- **Week 1 (3/17)**: Research the relevant field knowledge, set up project
repository.

- **Week 2 (3/24)**: Implement GEMM Component in C++. Set up CI to 
run **correctness tests** on basic GEMM functions.

- **Week 3 (3/31)**: Implement lookup table component.

- **Week 4 (4/7)**: Use SIMD instructions to accelerate table lookup.

- **Week 5 (4/14)**: Optimize memory management and implement different 
precision support. Add vendor library support in GEMM component.

- **Week 6 (4/21)**: Develop a benchmarking component and  documentation.

- **Week 7 (4/28)**: Integrate the API to evaluate this project and refine 
documentation.

- **Week 8 (5/5)**: Final optimizations and documentation updates.

- **Week 9 (5/12)**: Compare LUT-based GEMM with naive GEMM and vendor
libraries (MKL/Accelerate) and finish documentation.
 
- **Week 10 (5/19)**: Prepare for presentation.

References
==========

- **DeepGEMM:** 
https://openaccess.thecvf.com/content/CVPR2023W/ECV/papers/Ganji_DeepGEMM_Accel
erated_Ultra_Low-Precision_Inference_on_CPU_Architectures_Using_Lookup_CVPRW_20
23_paper.pdf

- **T-MAC:** https://arxiv.org/html/2407.00088v1
