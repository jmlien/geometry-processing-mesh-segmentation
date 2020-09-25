#include "per_face_feature.h"


/*

avg geodesic distance

Input:

V: Vectex positions of the mesh
F: Face indices of the mesh

Output:
avg_geodesic: a |F|*1  vector storing per face feature
*/


void per_face_avg_geodesic
(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, Eigen::VectorXd& avg_geodesic)
{
  // compute avg geodesic distance from the center of each face to all faces in the mesh
  // normalize the feature (avg_geodesic) between 0 and 1
}

/*
shape diameter function

Input:

V: Vectex positions of the mesh
F: Face indices of the mesh

Output:
avg_geodesic: a |F|*1 vector storing per face feature
*/

void per_face_SDF
(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, Eigen::VectorXd& SDF)
{
  //compute avg SDF value from the center of each face

  // normalize the feature (SDF) between 0 and 1
}
