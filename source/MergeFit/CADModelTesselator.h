#ifndef MFT_CAD_MODEL_TESSELATOR_H
#define MFT_CAD_MODEL_TESSELATOR_H

#include "CADModel.h"

namespace mft{

class CADModelTesselator{

public:

	/**
	 * @brief Tesselate a CAD model
	 * @param[in] cad the CAD model
	 * @param[in] ratio 
	 * @return Return 0 on successful execution
	 */
	int GenTriMesh(CADModelPtr& cad, const double& ratio = 0.0001);

	/**
	 * @brief Tesselate a CAD model
	 * @param[in] cad the CAD model
	 * @param[in] linear_deflection 
	 * @param[in] angular_deflection 
	 * @return Return 0 on successful execution
	 */
	int GenTriMesh(CADModelPtr& cad, const double& linear_deflection, const double& angular_deflection);

};

}

#endif