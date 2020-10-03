#include "similarity_matrix.h"

/*
Input:
V: Vectex positions of the mesh
F: Face indices of the mesh
per_face_features: |F|*1 feature vector

Output:
similarity: a symmetric |F|*|F| similarity matrix containing values [0,1];

- A value of 0 means a pair of faces are very different and should be classified into different clusters.
- A large value means a pair of faces have high similarity and should be classified into the same cluster.
- The matrix should be symmetric
- The diagonal should be all 0s

*/

void compute_similarity_maxtrix
(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F,
 const Eigen::VectorXd& per_face_features,
 Eigen::MatrixXd& similarity)
{

}
