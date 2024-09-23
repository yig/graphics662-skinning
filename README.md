# Advanced Graphics: Homework Assignment 2: Linear Blend Skinning with Bounded Biharmonic Weights

## Overview

In this assignment, you will implement linear blend skinning and automatic weight computation via [Bounded Biharmonic Weights](https://igl.ethz.ch/projects/bbw/). Your output can look like this:

![ginger bread person](tests/graph%20laplacian/woody5%20unbounded.png)

There is a demo you can interact with here:

<https://yig.github.io/graphics662-skinning/serve/>

## Setup

Download or clone this code repository. Don't fork it on GitHub, or else your code will be visible to everyone. If you want to work on it on GitHub, use <https://github.com/new/import> to create an independent, private copy by pointing it at this repository's URL and choosing "Private".

This project is written in C++ and makes use of the [Eigen](https://eigen.tuxfamily.org) matrix library. Documentation can be found [here](https://eigen.tuxfamily.org/dox/group__QuickRefPage.html). For part of this project you will use sparse matrices. You can find Eigen's documentation about that [here](https://eigen.tuxfamily.org/dox/group__TutorialSparse.html).

You will be compiling your code for the web (to WebAssembly) with [emscripten](https://emscripten.org). Follow [the instructions to install and activate the Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html). Emscripten provides the C++ compiler.

You will need [CMake](https://cmake.org/):
  * On Windows, you can use the CMake installer from the [official website](https://cmake.org/).
  * On macOS, use [Homebrew](https://brew.sh/): `brew install cmake`.
  * On Linux, use your distribution's package manager to install cmake.

## Compiling

1. Use Emscripten's CMake wrapper to setup your build system: `emcmake cmake -B build-dir -DCMAKE_BUILD_TYPE=Debug`. (I added a flag to compiler in debug mode. I strongly recommend this until your code is working.)
2. Compile with `cmake --build build-dir`.

> 🤖: For more information about using CMake, see [this guide](https://github.com/yig/GMU-CS425) I wrote for a different course (up through *Checkpoint 1 Upload*). Use `ccmake -B build-dir` to choose compile-time options. (Note the extra `c`.) One useful option is `CMAKE_BUILD_TYPE`. You can choose `Release` (optimized code generation), `Debug` (unoptimized code with debug information) or `RelWithDebInfo` (optimized code with debug information; may be hard to debug since compilers move code around when optimizing). You can pass options on the command line with `-D`, as in `cmake -B build-dir -DCMAKE_BUILD_TYPE=Debug`.

## Running

Run by serving the contents of the `serve` directory. For example,
    ```
    cd serve
    python3 -m http.server 8000
    ```
    and then browsing to <http://localhost:8000/>.

You don't need to re-start the server when you recompile. Just refresh your browser.

## Writing

Fill in the functions in the file `bbw.cpp`. That's the only file you need to change.

You can compare your output to the examples in tests directory. It
contains screenshots of correct solutions (next to the JSON files).
Once your `linear_blend_skin_2D()` function is working, your code should
produce identical output. You can test whether you generate the same
weights by loading the JSON test file and recalculating the weights.
To recalculate the weights, click “Add Handles” and then immediately
“Manipulate Handles”.

Feel free to enhance the interface if you so desire.

**THIS IS AN INDIVIDUAL, NOT A GROUP ASSIGNMENT. That means all code written for this assignment should be original! Code that is substantially the same will be considered cheating.** You may work on theoretical derivations with other students, but not code. In your `Notes.txt`, please note who deserves a star (who helped you with the assignment).

## Debugging

You can [debug WebAssembly directly in Chrome](https://developer.chrome.com/docs/devtools/wasm/). It works great! Compile in debug mode (`Debug` or `RelWithDebInfo`). Install the Chrome [C/C++ DevTools Support](https://chromewebstore.google.com/detail/cc++-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb) extension. Set breakpoints in your source code, which can be found in *Chrome Developer Tools > Sources > `file://`*. [Here is a screenshot from a different assignment](https://yig.github.io/graphics662-curves/docs/Chrome%20WASM%20DevTools.png). Make sure to "Disable cache" under *Chrome Developer Tools > Network* so that you can refresh the page after recompiling. (**N.B.** The cache is only disabled while the Developer Tools are open.) You can also debug [from inside VSCode](https://floooh.github.io/2023/11/11/emscripten-ide.html).

In addition, anything you print to standard output or standard error will appear in your browser's "Developer Tools > Console".

## Handing In

Create a file named `Notes.txt` in the folder. Describe any known issues or extra features. Name people in the class who deserve a star for helping you (not by giving your their code!).

When done, upload a zip of your entire directory. Do not include your build directory. It is large and unnecessary. You can zip whatever you have checked into git via: `git archive -o ../skinning.zip --prefix=skinning/ main`.


## Rubric

### 1. Linear blend skinning (20 points)

Write a function to evaluate linear blend skinning;
fill in `linear_blend_skin_2D()`. You are given a vector of $N$ 2D
points $\mathbf{p} _ i = [ x _ i, y _ i ]$ for $(i =
0$ to $N-1$), an array of $H$ $3\times 3$ transformation matrices $\mathbf{T} _ j$, 
and an $N$-by-$H$ matrix $\mathbf{W}$ of weights, where $\mathbf{W} _ {ij}$ is the influence of transformation $\mathbf{T} _ j$ on point $\mathbf{p} _ i$. You should return an 
array of transformed points $\mathbf{p} _ i'$, where  
$$\mathbf{p} _ i' = \sum _ {j = 0}^{H - 1} \mathbf{W} _ {ij}\mathbf{T} _ j\mathbf{p} _ i$$
The 2D vertices passed to `linear_blend_skin_2D()` are **not**
homogeneous. You must add a 1 at the end to see translations.  

### 2. Laplacian and Mass matrices (40 points)

Compute the Laplacian and Mass matrices; fill in
`laplacian_and_mass_matrices()`. You are given `vertices`, a sequence of $N$
2D points $\mathbf{p} _ i = [ x _ i, y _ i ]$ (equivalently, an $N$-by-2 matrix),
and `faces`, a sequence of $F$ triplets of integer indices into `vertices` (equivalently, an $F$-by-3 array). Each
row of `faces` contains the indices of the three vertices that make up
triangle $f$. Note that faces are always oriented counter-clockwise. You
should return two $N$-by-$N$ *sparse* matrices, `Laplacian` and `Mass`. The
simplest way to do this is to build what is known as the graph
Laplacian $L$. Each row of the graph Laplacian applies the
expression
$$\mathbf{p} _ i - \frac{1}{|N(i)|}\sum _ {j \in N(i)}\mathbf{p} _ j$$
when you multiply the matrix $L$ by a vector $\mathbf{p}$,
where $N(i)$ are the neighboring vertex indices to vertex $i$ (that is, there is
an edge between them).

As a matrix, it looks as follows. For every edge between vertex $i$ and $j$,
$L _ {ij} = \frac{- 1}{|N(i)|}$. The diagonal elements
$L _ {ii}$ are equal to minus the sum of the other elements in the row:
$$L _ {ii} = - \sum _ {j \neq i}L _ {ij}$$
What is the mass matrix? The mass matrix $M$ stores along its diagonal the
“mass” associated with each vertex. For the graph Laplacian, it’s
simply $|N(i)|$.

**Bonus (20 points)**: Create a Cotangent Laplacian matrix and Voronoi or Barycentric
mass matrix. This will make your code almost entirely agnostic to
irregular triangulations. Your weights will look better. It’s not really that hard.
See Section IIB in *On
Linear Variational Surface Deformation Methods* \[Botsch & Sorkine
2007\] or Section 3.3 and 3.5 in *Discrete Differential-Geometry
Operators for Triangulated 2-Manifolds* \[Meyer et al. 2003\].

### 3. Bounded Biharmonic Weights (40 points)

Implement Bounded Biharmonic Weights; fill in `bbw()`.
[*Bounded Biharmonic Weights for Real-Time Deformation* \[Jacobson et al. 2011\]](https://igl.ethz.ch/projects/bbw/)
are a technique
for automatically computing weights used in linear blend skinning. You
will implement a miniature version with only point handles. Bounded
biharmonic weights are computed by minimizing a Laplacian-based
energy:  
$$\min _ {w _ {i}}\int \| \Delta w _ {i}(\mathbf{p}) \|^2 \textrm{d}\mathbf{p}$$  
such that  
$$w _ {i}(\mathbf{h} _ {j}) = \delta _ {ij} \quad \text{ and } \quad 0 \leq w(p) \leq 1$$  
where
$w _ {i}$ are the weights for handle $i$, and the
Kronecker delta $δ _ {ij} = 1$ if $i=j$ and 0 otherwise. The
Kronecker delta constraint says that the weights for handle $i$ must
be 1 at the location of the handle and 0 at the location of all other
handles.

The discrete version of this, using your matrices from above, is:
$$\min _ {w _ {i}} w _ {i}^\top L^\top M L w _ {i}$$
such that  
$$w _ {i}\lbrack\mathbf{h} _ {j}\rbrack = \delta _ {ij} \quad \text{ and } \quad 0 \leq w _ i \leq 1$$  
Again, the $w _ i$ are the vector of weights for handle $i$.
The Kronecker delta constraint says that the weights for handle $i$
must be 1 at the vertex index of the handle and 0 at the vertex index
of all other handles.

**Implementation**: You will use a Quadratic Programming (QP) solver
to solve this optimization problem (see below). You will compute each
handle’s weights separately, modifying the Kronecker delta constraint
each time.
The QP solver I am recommending allows you to specify lower and upper bounds
and equality constraints directly.
(Many other QP solvers take more general input: one linear system (matrix and right-hand-side vector) each for the equality and inequality constraints.)
The result of each call to the QP solver is
one column of the \#vertices-by-#handles weights matrix $w$ you will
return. Before returning it, make sure to normalize each row so that
it adds up to 1. (Divide each row by its sum.)

## Quadratic Programming

Quadratic programming solves for the vector of unknowns "x" that
minimize the expression:
$$\frac{1}{2}\mathbf{x}^{\top}P\mathbf{x} + \mathbf{q}^{\top}\mathbf{x}$$
subject to the constraint that
$$A _ {eq}\mathbf{x} = \mathbf{b} _ {eq}$$
and
$$A _ {ieq}\mathbf{x} \geq \mathbf{b} _ {ieq}$$

The quadratic part of the name quadratic programming come from the fact
that the expressions they minimize,
$$\frac{1}{2}\mathbf{x}^{\top}P\mathbf{x} + \mathbf{q}^{\top}\mathbf{x}$$
are quadratic. (If you were to expand the matrix multiplication into the
individual variables that make up $\mathbf{x}$, you would see that the
polynomial is degree 2.)

From calculus class, we know that one way to minimize a function is to
set the derivative equal to zero. This works beautifully in the case of
quadratic polynomials.

(When taking derivatives of expressions involving matrices, [Mike Brookes's The Matrix Reference Manual](http://www.ee.ic.ac.uk/hp/staff/dmb/matrix/calculus.html) is very handy. These days, though, I prefer to do it using Frobenius norms. Here is a [cheat sheet](https://gist.github.com/yig/c1959ce997f1d2fd6f3f982cb482e060) for that.)

The programming part of the name is a [strange historical curiosity](http://en.wikipedia.org/wiki/Mathematical_optimization#History),
but for our purposes refers to the constraints. We have linear
equality constraints (in our case, the handle vertices must have value 0
or 1, depending on which one we are solving for), and linear inequality
constraints (weights must be between 0 and 1). You can take each linear
expressed and write it in matrix form.

(Remember that a ≤ constraint is the same as a ≥ constraint if you
negate both sides of the equation.) The $A _ {eq}$ and $A _ {ieq}$
matrices have the same number of columns as $P$, and
the number of rows equals the number of equality and inequality
constraint equations, respectively.

**WARNING**: I will deduct 10 points if your code is slow due to dense
matrix operations. You can write your code using dense matrices first,
but rewrite your code using sparse matrices to avoid the deduction.
Multiplication of dense matrices is where the speed difference will be
noticeable. There are two ways to build a sparse matrix:

1. Build a dense matrix (a `MatrixXd`) and convert it to a sparse one using `.sparseView()`.
This is not ideal, because it wastes $O(n^2)$ time and space.
(You can build a diagonal matrix as a `Eigen::VectorXd` and call `.asDiagonal()` to view it as a diagonal matrix. This is very efficient.)

2. Build triplets rows, cols, values, where the corresponding element implies a matrix $M$ such that
$M _ {\textrm{rows}[i], \textrm{cols}[i]} = \textrm{values}[i]$.
In Eigen, you can populate a `std::vector< Eigen::Triplet<double> >` and convert it to a sparse matrix via `.setFromTriplets( triplets.begin(), triplets.end() )`.


## Using a QP Solver

I have included [libigl](https://libigl.github.io)'s active set solver. You can read about it in the docs: [`active_set.h`](https://github.com/libigl/libigl/blob/main/include/igl/active_set.h). For this assignment, we can make do with only the lower and upper bounds and known rows. Your call would look like the following:

```c++
VectorXd Z = VectorXd::Zero( num_vertices );
igl::SolverStatus status = igl::active_set(
    A, B,
    known,
    Y,
    Eigen::SparseMatrix<double>(), Eigen::VectorXd(),
    Eigen::SparseMatrix<double>(), Eigen::VectorXd(),
    lx, ux,
    igl::active_set_params(),
    Z
    );
```

The empty matrices tell it that we aren't using the general linear $A_{eq}$ and $A_{ieq}$ constraints.
(You are free to do so if you choose.)
Note that the header documentation states that the `Z` output parameter can be passed empty, but that's not true.
It must be initialized to the correct size.


### Example

```c++
// Make a 4x4 identity matrix for the quadratic term in our energy expression.
int N = 4;
SparseMatrix<double> A( N, N );
A.setIdentity();

// Make a 4-vector of -1/2 for the linear term in our energy expression.
// The energy is minimized when the variables are equal to 0.5.
VectorXd B = -.5*VectorXd::Ones( N );

// Make a vector for lower and upper bounds
VectorXd lx = VectorXd::Zero( N );
VectorXd ux = VectorXd::Ones( N );

// Make vector with known indices and their values
VectorXi known( 2 );
VectorXd Y( 2 );
// We will know elements 1 and 3
known(0) = 1;
Y(0) = 0.125;

known(1) = 3;
Y(1) = 0.888;

std::cout << "Beginning QP solve\n";

VectorXd Z = VectorXd::Zero( N );
igl::SolverStatus status = igl::active_set(
    A, B,
    known,
    Y,
    Eigen::SparseMatrix<double>(), Eigen::VectorXd(),
    Eigen::SparseMatrix<double>(), Eigen::VectorXd(),
    lx, ux,
    igl::active_set_params(),
    Z
    );

std::cout << "Finished QP solve with status: " << status << '\n';

std::cout << Z << '\n';
```

---

The program prints:

```
Beginning QP solve
Finished QP solve with status: 0
   -0
0.125
   -0
0.888
```
