#include "similarity_matrix.h"

/*
Input:
V: Vectex positions of the mesh
F: Face indices of the mesh
per_face_features: |F|*1 feature vector

Output:
similarity: a symmetric |F|*|F| similarity matrix containing values [0,1];
a pair of faces has value 0 if they are a perfect match.
*/

void compute_similarity_maxtrix
(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F,
 const Eigen::VectorXd& per_face_features,
 Eigen::MatrixXd& similarity)
{

}
