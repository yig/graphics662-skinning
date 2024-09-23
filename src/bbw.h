#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>

namespace bbw {

typedef double real;

/*
Given:
    `vertices`: An array of 2D points passed as a #vertices-by-2 matrix
    `weights`: A matrix of #vertices-by-#transforms weights
    `transforms`: An array of 3x3 transformation matrices
Returns:
    The linear blend skinning deformation of each vertex as a #vertices-by-2 matrix.

NOTE: The input 2D vertices are not homogeneous.
      They are just [ [ x0, y0 ], [ x1, y1 ], ... ].
      You must convert them to homogeneous coordinates to see the effects of translation.
      You must return non-homogeneous 2D coordinates.
*/
Eigen::MatrixXd linear_blend_skin_2D(
    const Eigen::MatrixXd& vertices,
    const Eigen::MatrixXd& weights,
    const std::vector< Eigen::Matrix3d >& transforms
    );

/*
Given:
    `vertices`: An array of N 2D points pi = [xi, yi] (as an N-by-2 matrix)
    `faces`: An array of F triplets of integer indices into vertices, where the triplet faces[f][0], faces[f][1], faces[f][2] are the indices of the three vertices that make up triangle f
    `handles`: a vector of indices into `vertices` specifying the handle vertices
    `laplacian_mode`: a string which will be one of "graph" or "cotangent"
    `solver_mode`: a string which will be one of "bounded" or "unbounded"
Returns:
    An #vertices-by-#handles weight matrix W, where W[i][j] is the influence weight of the j-th handle on the i-th vertex. Each row of W must sum to 1.

If `solver_mode` is "bounded", apply inequality constraints so that the weights are all between 0 and 1.
*/
Eigen::MatrixXd bbw(
    const Eigen::MatrixXd& vertices,
    const Eigen::MatrixXi& faces,
    const std::vector<int>& handles,
    const std::string& laplacian_mode,
    const std::string& solver_mode
    );

}
