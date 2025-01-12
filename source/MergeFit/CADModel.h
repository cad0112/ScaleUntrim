#ifndef MFT_CAD_MODEL_H
#define MFT_CAD_MODEL_H

#include <TopoDS_Shape.hxx>
#include <memory>

namespace mft{

class CADModel{

public:

	explicit CADModel(const TopoDS_Shape& i_shape); 

	const TopoDS_Shape& Shape(); 

protected:

	TopoDS_Shape mShape;

};

}

using CADModelPtr=std::shared_ptr<mft::CADModel>;

#endif