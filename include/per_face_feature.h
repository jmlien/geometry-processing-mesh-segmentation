#pragma once

#include <igl/exact_geodesic.h>
#include <igl/shape_diameter_function.h>
#include <igl/barycenter.h>

/*

avg geodesic distance

Input:

V: Vectex positions of the mesh
F: Face indices of the mesh

Output:
avg_geodesic: a |F|*1  vector storing per face feature
*/


void per_face_avg_geodesic
(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, Eigen::VectorXd& avg_geodesic);

/*
shape diameter function

Input:

V: Vectex positions of the mesh
F: Face indices of the mesh

Output:
avg_geodesic: a |F|*1 vector storing per face feature
*/

void per_face_SDF
(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, Eigen::VectorXd& SDF);
