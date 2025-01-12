#ifndef _MFT_QUAD_MESH_IO_H_
#define _MFT_QUAD_MESH_IO_H_

#include "QuadMesh.h"

namespace mft {

class QuadMeshIO {

public:

    int SaveVTK(QuadMeshPtr& mesh, const char* output_path);

    int SaveOBJ(QuadMeshPtr& mesh, const char* output_path);

    int SavePatchVTK(QuadMeshPtr& mesh, const char* output_patch_file, const char* output_patch_information);

};

}

#endif
