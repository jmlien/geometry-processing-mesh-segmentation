#pragma once
#include <Eigen/Core>
#include <Eigen/Sparse>

/*
Input:
F: Face indices of the mesh
per_face_features: |F|*1 feature vector

Output:
similarity: a symmetric |F|*|F| similarity matrix
*/

void compute_similarity_maxtrix
(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, const Eigen::VectorXd& per_face_features,
 Eigen::MatrixXd& similarity);
