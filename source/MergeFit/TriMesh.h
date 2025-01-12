#ifndef MFT_TRI_MESH_MODEL_H
#define MFT_TRI_MESH_MODEL_H

#include <Eigen/Core>
#include <memory>

namespace mft{

class TriMesh{

public:

	Eigen::MatrixXd& VMat();
	Eigen::MatrixXi& FMat();

protected:

	Eigen::MatrixXd mV; // 3*vertex_num
	Eigen::MatrixXi mF; // 3*face_num

};

}

using TriMeshPtr=std::shared_ptr<mft::TriMesh>;

#endif