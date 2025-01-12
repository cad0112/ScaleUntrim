#ifndef MFT_CAD_MODEL_IO_H
#define MFT_CAD_MODEL_IO_H

#include "CADModel.h"

namespace mft{

class CADModelIO{

public:

	/**
	 * @brief read a CAD model file
	 * @param[in] cad_file_path input CAD file path (*.stp/step/igs/iges)
	 * @return Return nullptr if failed
	 */
	CADModelPtr Load(const char* cad_file_path);

};

}

#endif