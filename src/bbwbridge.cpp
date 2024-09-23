#include "bbw.h"

#include <emscripten.h>
#include <emscripten/bind.h>

namespace bbwbridge {

// Helper function to convert JavaScript array to Eigen::MatrixXd
template< typename T >
T EigenMatrix_From_JSArrays( const emscripten::val& jsArray ) {
    int rows = jsArray["length"].as<int>();
    int cols = jsArray[0]["length"].as<int>();
    T result(rows, cols);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result(i, j) = jsArray[i][j].as<typename T::Scalar>();
        }
    }
    
    return result;
}

// Helper function to convert Eigen::Matrix to JavaScript array
template< typename T >
emscripten::val JSArrays_From_EigenMatrix( const T& mat ) {
    emscripten::val jsResult = emscripten::val::array();
    for (int i = 0; i < mat.rows(); ++i) {
        emscripten::val row = emscripten::val::array();
        for (int j = 0; j < mat.cols(); ++j) {
            row.set( j, mat(i, j) );
        }
        jsResult.set(i, row);
    }
    
    return jsResult;
}

// Helper function to convert JavaScript array to std::vector<int>
template< typename T >
std::vector<T> Vector_From_JSArray(const emscripten::val& jsArray) {
    int size = jsArray["length"].as<int>();
    std::vector<T> result(size);
    
    for (int i = 0; i < size; ++i) {
        result[i] = jsArray[i].as<T>();
    }
    
    return result;
}

// Helper function to convert JavaScript array to std::vector<Eigen::Matrix3d>
std::vector<Eigen::Matrix3d> VectorMatrix3d_From_JSArrays( const emscripten::val& jsArray ) {
    int size = jsArray["length"].as<int>();
    std::vector<Eigen::Matrix3d> result(size);
    
    for( int i = 0; i < size; ++i ) {
        result.at(i) = EigenMatrix_From_JSArrays<Eigen::Matrix3d>( jsArray[i] );
    }
    
    return result;
}

// Wrapper function that can be called from JavaScript
emscripten::val wrapped_linear_blend_skin_2D( emscripten::val jsVertices, emscripten::val jsWeights, emscripten::val jsTransforms ) {
    Eigen::MatrixXd vertices = EigenMatrix_From_JSArrays<Eigen::MatrixXd>( jsVertices );
    Eigen::MatrixXd weights = EigenMatrix_From_JSArrays<Eigen::MatrixXd>( jsWeights );
    std::vector<Eigen::Matrix3d> transforms = VectorMatrix3d_From_JSArrays( jsTransforms );
    
    Eigen::MatrixXd result = bbw::linear_blend_skin_2D( vertices, weights, transforms );
    
    return JSArrays_From_EigenMatrix( result );
}

emscripten::val wrapped_bbw( emscripten::val jsVertices, emscripten::val jsFaces, emscripten::val jsHandles, std::string laplacianMode, std::string solverMode ) {
    Eigen::MatrixXd vertices = EigenMatrix_From_JSArrays<Eigen::MatrixXd>( jsVertices );
    Eigen::MatrixXi faces = EigenMatrix_From_JSArrays<Eigen::MatrixXi>( jsFaces );
    std::vector<int> handles = Vector_From_JSArray<int>(jsHandles);
    
    Eigen::MatrixXd result = bbw::bbw(vertices, faces, handles, laplacianMode, solverMode);
    
    return JSArrays_From_EigenMatrix( result );
}

}


EMSCRIPTEN_BINDINGS(bbwbridge) {
    emscripten::function( "linear_blend_skin_2D", &bbwbridge::wrapped_linear_blend_skin_2D );
    emscripten::function( "bbw", &bbwbridge::wrapped_bbw );
}
