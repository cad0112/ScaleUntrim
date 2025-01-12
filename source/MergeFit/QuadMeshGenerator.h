#ifndef _QUAD_MESH_GENERATOR_H_
#define _QUAD_MESH_GENERATOR_H_

// eigen includes
#include <Eigen/Core>
#include <Eigen/Dense>

#include "QuadMesh.h"
#include "TriMesh.h"

namespace mft {

class QuadMeshGenerator {

public:

    QuadMeshPtr GenMesh(TriMeshPtr& tri_mesh, 
                        double magnitude_factor, 
                        int preserve_sharp, 
                        int preserve_boundary, 
                        int minimum_cost, 
                        int adaptive_scale,
                        double angle);

    void meshinformation(std::vector<Vector3d> &V_, std::vector<Vector4i> &F_);

    std::vector<Vector3d> Ver;
    std::vector<Vector4i> Fer;

};

}

#endif
