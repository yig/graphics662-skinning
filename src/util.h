#pragma once

#include "bbw.h"

namespace bbw {

/*
Given:
    P, q: a sparse matrix P and vector q for an expression in terms of unknowns x: .5 x^T P x + q^T x
    indices, values: vectors containing indices into x and corresponding constrained values
Returns:
    A modified [ P, q ] such that the value constraints are incorporated and the matrix stays symmetric.
*/
inline
std::pair< Eigen::SparseMatrix<real>, Eigen::VectorXd > update_quadratic_and_linear_with_equality_constraints(
    const Eigen::SparseMatrix<real>& P,
    const Eigen::VectorXd& q,
    const std::vector<int>& indices,
    const std::vector<real>& values
    )
{
    using namespace Eigen;
    
    // Update the right-hand-side elements wherever a fixed degree-of-freedom
    // is involved in a row (the columns of constrained degrees-of-freedom).
    // Do this via:
    //     q_out = q + P*R
    // where R is a row matrix containing the constrained values.
    // var R = numeric.ccsScatter( [ numeric.rep( [ indices.length ], 0. ), indices, values ] );
    // var q_out = numeric.sub( q, numeric.ccsFull( numeric.ccsDot( R, P ) ) );
    int n = q.size();
    VectorXd R = VectorXd::Zero(n);
    for( int i = 0; i < indices.size(); ++i ) {
        R( indices.at(i) ) = values.at(i);
    }
    VectorXd q_out = q + P.transpose() * R;
    
    // Set corresponding entries of the right-hand-side vector to the constraint value.
    for( int i = 0; i < indices.size(); ++i ) {
        q_out( indices.at(i) ) = values.at(i);
    }
    
    // Finally, zero the constrained rows and columns, and set the diagonal to 1.
    
    // Zero the constrained rows and columns.
    VectorXd D = VectorXd::Ones(n);
    for( int index : indices ) {
        D( index ) = 0.0;
    }
    
    // SparseMatrix<real> D_sparse = D.asDiagonal();
    SparseMatrix<real> P_out = D.asDiagonal() * P * D.asDiagonal();
    
    // Set the diagonal to 1.
    for( int index : indices ) {
        P_out.coeffRef( index, index ) = 1.0;
    }
    
    return {P_out, q_out};
}

}
