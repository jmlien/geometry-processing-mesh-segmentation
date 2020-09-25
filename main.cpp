#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/PI.h>
#include <iostream>
#include <sstream>

#define DEBUG 0

//headers from this project
#include "per_face_feature.h"
#include "similarity_matrix.h"
#include "spectral_clustering.h"


//removing the extension of a filename
std::string strip_filename(const std::string & name); //defined later in this file

int main(int argc, char *argv[])
{
  using namespace std;

  int K=5;         //number of segments
  bool sdf=true;   //feature type: sdf or geo
  bool viz=false;  //visualize the results?
  string filename;

  if(argc<2){
    cerr<<"usage: "<<argv[0]<<" [-g] [-k #] [-sdf|-geo] mesh_file (*obj or *off)\n"
             <<"    -k #: number of segments (default is 5)\n"
             <<"    -g: turn on visualization\n"
             <<"    -sdf: segment using shape diameter function (default)\n"
             <<"    -geo: segment using average geodesic distance"<<endl;
    return 1;
  }

  for(int i=1;i<argc;i++){
    if(argv[i][0]=='-'){
      string opt(argv[i]);
      if(opt=="-k") K=atoi(argv[++i]);
      else if(opt=="-g") viz=true;
      else if(opt=="-sdf") sdf=true;
      else if(opt=="-geo") sdf=false;
      else { cerr<<"! Warning: Unknown flag: ("<<opt<<")"<<endl; }
    }
    else filename=argv[i];
  }

  if(filename.empty()){
    cerr<<"! Error: no mesh file given"<<endl;
    return 1;
  }

  //data
  Eigen::MatrixXd V;                 // vetex positions
  Eigen::MatrixXi F;                 // face indices
  Eigen::VectorXd per_face_feature;  // per face feature
  Eigen::MatrixXd similarity;        // face-face similarity
  Eigen::MatrixXi Fclusters;         // face  cluster indices
  Eigen::VectorXi CC;                // cluster counts

  //read in mesh from file
  if(!igl::read_triangle_mesh(filename, V, F)){
    cerr<<"! Error: Cannot read "<<filename<<endl;
    return 1;
  }

  //start to decompose!
  cout<<"- Decomposing "<<filename<<" into "<<K<<" piece(s)"<<endl;

  //compute per face features
  if(sdf) per_face_SDF(V,F,per_face_feature);
  else per_face_avg_geodesic(V,F,per_face_feature);

#if DEBUG
  if(viz)
  {
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    //draw faces in color according to the segments
    Eigen::MatrixXd C=Eigen::MatrixXd::Zero(F.rows(),3);
    Eigen::RowVector3d red(1,0,0), blue(0,0,1);
    for(int i=0;i<F.rows();i++)
      C.row(i)=per_face_feature[i]*red+(1-per_face_feature[i])*blue;
    viewer.data().set_colors(C);
    viewer.launch();
  }
#endif

  //compute similarity matrix
  compute_similarity_maxtrix(V,F,per_face_feature,similarity);

  //spectral_clustering
  spectral_clustering(K, V, F, similarity, Fclusters, CC);

  //save the clusters into separate files
  int last_row=0;
  for(int i=0;i<K;i++){
    string outfile=strip_filename(filename);
    stringstream ss;
    ss<<outfile<<"-"<<setfill('0') << setw(3)<<i<<".obj";

    cout<<"CC["<<i<<"] has "<<CC[i]<<" faces"<<endl;
    Eigen::MatrixXi cluster = Fclusters.block(last_row, 0, CC[i], Fclusters.cols());
    igl::write_triangle_mesh(ss.str(),V,cluster);
    last_row+=CC[i];
  }

  if(viz)
  {
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, Fclusters);
    // This function is called every time a keyboard button is pressed
    viewer.callback_key_down = [&](igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)->bool
    {
      //upper cases only
      if (key == 'C') //change colors of the segmented pieces
      {
        //draw faces in color according to the segments
        Eigen::MatrixXd C=Eigen::MatrixXd::Zero(F.rows(),3);
        int last_row=0;
        for(int i=0;i<K;i++){
          Eigen::RowVector3d color=0.5*Eigen::RowVector3d::Random().array() + 0.5;
          C.block(last_row, 0, CC[i], 3).rowwise()+=color;
          last_row+=CC[i];
        }
        viewer.data().set_colors(C);
        return true;
      }
      return false;
    };

    viewer.callback_key_down(viewer,'C',0); //simulate pressing down 'C'
    return viewer.launch();
  }

  return 0; //done!
}


//get rid of the beginning and end of a filename
std::string strip_filename(const std::string & name)
{
    int start = name.find_last_of("/")+1;
    if (start == std::string::npos)
      start = name.find_last_of("\\") + 1;
    if (start == std::string::npos)
      start = 0;
    int end = name.find_last_of(".");
    if (end == std::string::npos)
      end = name.length();
    return name.substr(start, end - start);
}
