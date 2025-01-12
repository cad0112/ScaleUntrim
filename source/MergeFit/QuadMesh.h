#ifndef MFT_QUAD_MESH_MODEL_H
#define MFT_QUAD_MESH_MODEL_H

#include "parametrizer.hpp"
#include <memory>

namespace mft{

class QuadMesh{

public:

	qflow::Parametrizer& Field();

protected:

	qflow::Parametrizer mField;

};

}

using QuadMeshPtr=std::shared_ptr<mft::QuadMesh>;

#endif