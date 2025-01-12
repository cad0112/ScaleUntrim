
#include "CmdTesselateCAD.h"
#include "CADModelIO.h"
#include "CADModelTesselator.h"
#include "TriMeshIO.h"

#include <iostream>
#include <fstream>

namespace mft
{

struct CmdTesselateCADParam
{
    double tri_tol = 1.e-4;
};

void ParseCmdTesselateCADParam(const char* config_file_path, CmdTesselateCADParam& para)
{
    std::ifstream fin;
    fin.open(config_file_path);
    if (fin.is_open())
    {
        std::string parameter_type;
        std::string temp;
        while (fin >> parameter_type)
        {
            if (parameter_type == "tri_mesh_tolerance:")
                fin >> para.tri_tol;
            else
                fin >> temp;
        }
        fin.close();
    }
}

int CmdTesselateCAD::Run(const char* cad_file_path, const char* tri_mesh_file_path, const char* config_file_path)
{
	// 0. Parse the parameter
    CmdTesselateCADParam para;
    ParseCmdTesselateCADParam(config_file_path, para);

	// 1. read cad
	CADModelPtr cad = mft::CADModelIO().Load(cad_file_path);
    if (cad == nullptr)
        return 1;

	// 2. gen tri mesh
    int res = mft::CADModelTesselator().GenTriMesh(cad, para.tri_tol);
    if (res != 0)
        return 1;

    // 3. save tri mesh file
    res = mft::TriMeshIO().SaveStl(cad, tri_mesh_file_path);
    if (res != 0)
        return 1;

	return 0;
}

} // namespace mft


