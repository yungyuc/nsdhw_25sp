===================================================================
Computational Geometry Library for Convex Hull Computation
===================================================================

Basic Information
=================

Computational geometry is a branch of computer science that deals with algorithms 
for solving geometric problems. This project will focus on developing a library 
for computing convex hulls in 2D and 3D, analyzing their performance and accuracy.
The goal is to create an efficient and robust library that can handle various 
applications in pattern recognition, computer graphics, and robotics.


Problem to Solve
================

Computing convex hulls is a fundamental problem in computational geometry with 
numerous applications. However, implementing efficient and robust algorithms for 
convex hull computation can be challenging. This project aims to address this issue 
by providing a well-designed library with optimized algorithms and thorough 
performance analysis.


Prospective Users
=================

This computational geometry library will be beneficial to researchers, developers, 
and practitioners in various fields, including:

* Pattern recognition: Identifying shapes and features in images and data.
* Computer graphics: Generating realistic 3D models and animations.
* Robotics: Planning collision-free paths for robots.
* Geographic information systems (GIS): Analyzing spatial data and creating maps.


System Architecture
===================

* Using :cpp:class:`!C++` to implement the algorithm.
* Using :cpp:class:`!pybind11` to make API in python.


API Description
===============

The library will provide a user-friendly API for convex hull computation and related 
operations. Key functionalities include:

* `ConvexHull2D(points)`: Computes the convex hull of a set of 2D points.
* `ConvexHull3D(points)`: Computes the convex hull of a set of 3D points.
* `Area(polygon)`: Calculates the area of a polygon.
* `Volume(polyhedron)`: Calculates the volume of a polyhedron.
* `IsInside(point, polygon)`: Checks if a point is inside a polygon.
* `Intersection(polygon1, polygon2)`: Computes the intersection of two polygons.


Engineering Infrastructure
==========================

* Use :cpp:class:`!CMake` to build the binaries.
* Use :cpp:class:`!git` for version control.
* Use :cpp:class:`!pytest` for testing the performance and correctness.


Schedule
========

Week 1 (mm/dd):
  Project setup and literature review.

Week 2 - 3 (mm/dd):
  Implementation of 2D convex hull algorithms and 3D convex hull algorithms.

Week 4 - 5 (mm/dd):
  Performance evaluation and comparison of algorithms

Week 6- 7 (mm/dd):
  API design and implementation

Week 8 (mm/dd):
  Project presentation and submission.


References
==========

* https://en.wikipedia.org/wiki/Convex_hull_algorithms