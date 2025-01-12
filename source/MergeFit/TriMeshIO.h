#ifndef TRI_MESH_IO_H
#define TRI_MESH_IO_H

#include "CADModel.h"
#include "TriMesh.h"

namespace mft{

class TriMeshIO{

public:

	/**
	 * @brief convert an STL file to an OBJ file 
	 * @param[in] stl_file_path input STL file path (*.stl)
	 * @param[in] obj_file_path output OBJ file path (*.obj)
	 * @return Return 0 on successful execution
	 */
	int ConvertStlToObj(const char* stl_file_path, const char* obj_file_path);

	int SaveStl(const CADModelPtr& cad, const char* stl_file_path);

	TriMeshPtr LoadOBJ(const char* obj_file_path);

	TriMeshPtr LoadSTL(const char* stl_file_path);

};

}

#endif