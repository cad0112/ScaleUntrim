#ifndef CMD_RECONTRUCT_CAD_H
#define CMD_RECONTRUCT_CAD_H

namespace mft{

class CmdReconstructCAD{

public:

	/**
	 * @param[in] tri_mesh_file_path input tri mesh file path (*.stl)
	 * @param[in] cad_file_path output cad file path (*.step)
	 * @param[in] config_file_path config file path
	 * @return Return 0 on successful execution
	 */
	static int Run(const char* tri_mesh_file_path, const char* cad_file_path, const char* config_file_path);

};

}

#endif