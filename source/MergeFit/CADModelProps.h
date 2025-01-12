#ifndef MFT_CAD_MODEL_PROPS_H
#define MFT_CAD_MODEL_PROPS_H

#include "CADModel.h"

namespace mft{

class CADModelProps{

public:

	/**
	 * @brief Compute area of a CAD model
	 */
	double ComputeArea(CADModelPtr& cad);

};

}

#endif