#include "spectral_clustering.h"
#include <Eigen/Eigenvalues>
#include <iostream>
#include <ctime>

using namespace std;

//these functions are defined later in this file
void repair_clusters(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, Eigen::MatrixXi& sF, Eigen::VectorXi& sFC);
std::vector<std::vector<int> > spectral_clustering(const Eigen::MatrixXd& A, int K);
std::vector<std::vector<int> > kmeans_cluster(const Eigen::MatrixXd& data, int K);

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
 Eigen::MatrixXi& sF, Eigen::VectorXi& sFC)
{

	//Requirements
	// - similar faces are clustered together based on the similarity matrix
	// - faces in a cluster must be contiguous and form a single connected component


  //dummy segmentation, replace the following code
  int cluster_size=F.rows()/K;
  sFC=Eigen::VectorXi::Constant(K,cluster_size);
  sFC[K-1]=F.rows()-cluster_size*(K-1);
  sF=F;

  //YOUR CODE HERE

  //make sure that faces in the same cluster are connected
  repair_clusters(V,F,sF,sFC);
}


/// repair clusters so that each cluster contains a compact connected component
void repair_clusters
(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, Eigen::MatrixXi& sF, Eigen::VectorXi& sFC)
{
  //YOUR CODE HERE
}

/**
* Spectral clustering into K clusters
*
* @param A 		the affinity matrix
* @param K    the number of clusters
*
* @return		   ordered set of data row indices for each cluster
*/

std::vector<std::vector<int> > spectral_clustering(const Eigen::MatrixXd& A, int K)
{
	// compute normalized laplacian
  // using Ng, Jordan and Weiss, "On Spectral Clustering: Analysis and an Algorithm", NIPS 2002
  Eigen::VectorXd S=A.colwise().sum().array().pow(-0.5);
  Eigen::MatrixXd D=S.asDiagonal();
  Eigen::MatrixXd I=Eigen::MatrixXd::Identity(A.rows(),A.cols());
  Eigen::MatrixXd L = I - (D* A *D);

  //solve, eigenvalues are sorted in increasing order
  cout<<"- Solving Eigen Decomposition"<<endl;
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(L);
	Eigen::VectorXd E = es.eigenvalues();
	Eigen::MatrixXd V = es.eigenvectors();

  // keep only the k-principal components (Eigen's eigenvectors are colwise)
  V = V.block(0,1,V.rows(),K);

  //normalize V
  Eigen::VectorXd norm = V.array().pow(2).matrix().rowwise().sum();
  for (unsigned int k = 0; k < K; ++k) V.col(k) = V.col(k).array() / norm.array();

  return kmeans_cluster(V, K);
}

/**
 * kmeans clustering
 * based on kmeans matlab script by Ian T Nabney (1996-2001)
 *
 * @param data 	affinity matrix
 * @oaram ncentres	number of clusters
 * @return		ordered set of data row indices for each cluster
 * 			    	(order is based on distance to cluster centre)
 */
std::vector<std::vector<int> >
kmeans_cluster(const Eigen::MatrixXd& data, int ncentres)
{
  std::srand(std::time(nullptr));
  //std::srand(0); //use this to debug so you get the same results

	int ndims = data.cols();
	int ndata = data.rows();

	Eigen::MatrixXd centres = Eigen::MatrixXd::Zero(ncentres, ndims);
	Eigen::MatrixXd old_centres;

	std::vector<int> rands;
	for (int i=0; i < ncentres; i++)
  {
		//randomly initialise centers
		bool flag;
		do {
			flag = false;
			int randIndex = std::rand()%ndata;
			//make sure same row not chosen twice
			for (unsigned int j=0; j < rands.size(); ++j) {
				if (randIndex == rands[j]) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				centres.row(i) = data.row(randIndex);
				rands.push_back(randIndex);
			}
		} while (flag);
	}
	Eigen::MatrixXd id = Eigen::MatrixXd::Identity(ncentres, ncentres);
	//maps vectors to centres.
	Eigen::MatrixXd post(ndata, ncentres);

	Eigen::VectorXd minvals(ndata);

	double old_e = 0;
	int niters = 100;
	for (int n=0; n < niters; n++) {
		//Save old centres to check for termination
		old_centres = centres;

		// Calculate posteriors based on existing centres
		Eigen::MatrixXd d2(ndata, ncentres);
		for (int j = 0; j < ncentres; j++) {
			for (int k=0; k < ndata; k++) {
				d2(k,j) = (data.row(k)-centres.row(j)).squaredNorm();
			}
		}

		int r,c;
		// Assign each point to nearest centre
		for (int k=0; k < ndata; k++) {
			//get centre index (c)
			minvals[k] = d2.row(k).minCoeff(&r, &c);
			//set centre
			post.row(k) = id.row(c);
		}

		Eigen::VectorXd num_points = post.colwise().sum();
		// Adjust the centres based on new posteriors
		for (int j = 0; j < ncentres; j++) {
			if (num_points[j] > 0) {
				Eigen::MatrixXd s = Eigen::MatrixXd::Zero(1,ndims);
				for (int k=0; k<ndata; k++) {
					if (post(k,j) == 1) {
						s += data.row(k);
					}
				}
				centres.row(j) = s / num_points[j];
			}
		}

		// Error value is total squared distance from cluster centres
		double e = minvals.sum();
		double ediff = fabs(old_e - e);
		double cdiff = (centres-old_centres).array().abs().maxCoeff();
		//std::cout << "Cycle " << n << " Error " << e << " Movement " << cdiff << ", " << ediff << std::endl;

		if (n > 1) {
			//Test for termination
			if (cdiff < 0.0000000001 && ediff < 0.0000000001) {
				break;
			}
		}
		old_e = e;
	}

	//------- finished kmeans ---------

	//find the item closest to the centre for each cluster
	std::vector<std::vector<int> > clustered_items;
	for (int j=0; j < ncentres; j++) {
		//put data into map (multimap because minvals[k] could be the same for multiple units)
		std::multimap<double,int> cluster;
		for (int k=0; k < ndata; k++) {
			if (post(k,j) == 1) {
				cluster.insert(std::make_pair(minvals[k], k));
			}
		}
		//extract data from map
		std::vector<int> units;
		//the map will be sorted based on the key (the minval) so just loop through it
		//to get set of data indices sorted on the minval
		for (std::multimap<double,int>::iterator it = cluster.begin(); it != cluster.end(); it++) {
			units.push_back(it->second);
		}

		clustered_items.push_back(units);
	}
	//return the ordered set of item indices for each cluster centre
	return clustered_items;
}
