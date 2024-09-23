#include "bbw.h"
#include "util.h"

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>
#include <utility> // std::pair
#include <tuple> // std::tie
#include <iostream> // std::cerr
#include <cmath> // std::sqrt

#include <cassert>
namespace Eigen { using namespace placeholders; }
#include <igl/active_set.h>

using namespace Eigen;

namespace bbw {

/*
Given:
    `vertices`: An array of N 2D points pi = [xi, yi] (as an N-by-2 matrix)
    `faces`: An array of F triplets of integer indices into vertices, where the triplet faces[f][0], faces[f][1], faces[f][2] are the indices of the three vertices that make up triangle f
Returns:
    Two N-by-N sparse matrices in a pair: Laplacian, Mass
*/
std::pair< SparseMatrix<real>, SparseMatrix<real> > laplacian_and_mass_matrices(
    const MatrixXd& vertices,
    const MatrixXi& faces
    )
{
    /// 1 Create an N-by-N matrix A, where N is the number of vertices, that is initially zero.
    /// 2 Iterate over all edges (i,j), setting a 1 at the corresponding (i,j) and (j,i) location in A.
    /// 3 Create an N-by-N diagonal mass matrix M, where the i-th diagonal is the sum of the i-th row of A.
    /// 4 The Laplacian matrix L is inverse( M )*(M - A). In other words,
    ///   it is (M-A) followed by dividing each row by its diagonal element.
    
    /// Add your code here.
    
    SparseMatrix<real> L, M;
    return { L, M };
}

MatrixXd bbw(
    const MatrixXd& vertices,
    const MatrixXi& faces,
    const std::vector<int>& handles,
    const std::string& laplacian_mode,
    const std::string& solver_mode
    )
{
    /// 1 Create the laplacian L and mass M matrices.
    /// 2 The bilaplacian B is L.T * M * L.
    /// 3 Create the constraint matrix. There will be an equality constraint for
    ///   every handle vertex, and 2 inequality constraints for the remaining vertices.
    /// 4 Solve once for each handle, setting each handles constraint value to 1 in turn.
    /// 5 Normalize each vertex's weights so that they sum to 1.
    
    /// Add your code here.
    
    // As a placeholder, give all handles equal weight.
    MatrixXd W = (1.0/handles.size())*MatrixXd::Ones( vertices.rows(), handles.size() );
    return W;
}

MatrixXd linear_blend_skin_2D(
    const MatrixXd& vertices,
    const MatrixXd& weights,
    const std::vector< Matrix3d >& transforms
    )
{
    MatrixXd result = MatrixXd::Zero( vertices.rows(), vertices.cols() );
    
    /// Add your code here.
    
    result = vertices;
    return result;
}

}
