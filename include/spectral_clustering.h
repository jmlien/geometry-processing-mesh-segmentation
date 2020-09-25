#pragma once
#include <Eigen/Core>
#include <Eigen/Sparse>

/*
Spectral clustering of the faces
- use similarity matrix and face connectivity to determine the clusters

Input:

K: number of clusters
V, F: the mesh
similarity: a |F|*|F| symmetric matrix

Output:

sF: A face index matrix of size |F|*3. Faces in the same cluster are in consecutive rows of sF
sFC: A K*1 vector. Each element indicates the size of each cluster stored in sF
*/

void spectral_clustering
(int K, const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, const Eigen::MatrixXd& similarity,
 Eigen::MatrixXi& sF, Eigen::VectorXi& sFC);
