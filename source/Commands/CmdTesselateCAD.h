#ifndef CMD_TESSELATE_CAD_H
#define CMD_TESSELATE_CAD_H

namespace mft{

class CmdTesselateCAD{

public:

	/**
	 * @param[in] cad_file_path input cad file path (*.step)
	 * @param[in] tri_mesh_file_path output tri mesh file path (*.stl)
	 * @param[in] config_file_path config file path
	 * @return Return 0 on successful execution
	 */
	static int Run(const char* cad_file_path, const char* tri_mesh_file_path, const char* config_file_path);

};

}

#endif