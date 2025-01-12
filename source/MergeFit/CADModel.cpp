#include "CADModel.h"

namespace mft{

CADModel::CADModel(const TopoDS_Shape &i_shape) : mShape(i_shape)
{
}

const TopoDS_Shape& CADModel::Shape()
{
    return mShape;
} 

}