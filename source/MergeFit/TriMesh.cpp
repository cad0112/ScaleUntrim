#include "TriMesh.h"

namespace mft{

Eigen::MatrixXd &TriMesh::VMat()
{
    return mV;
}

Eigen::MatrixXi &TriMesh::FMat()
{
    return mF;

}

}
