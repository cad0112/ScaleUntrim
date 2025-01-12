
#include "CmdReconstructCAD.h"
#include "TriMeshIO.h"
#include "QuadMeshGenerator.h"
#include "QuadMeshIO.h"
#include "QuadMeshSpliter.h"

#include "multi_patch_fit.h"
#include "GenerateShell.h"

#include <iostream>

namespace mft
{

struct CmdReconstructCADParam
{
    std::string temp_dir = "";
    double magnitude_factor = -1.0;
    double angle_tol = 60.;
    double fit_tol = 1.e-2;

    int preserve_sharp = 1;
    int preserve_boundary = 1;
    int minimum_cost = 1;
    int adaptive_scale = 1;
};

void ParseCmdReconstructCADParam(const char* config_file_path, CmdReconstructCADParam& para)
{
    ifstream fin;
    fin.open(config_file_path);
    if (fin.is_open())
    {
        std::string parameter_type;
        std::string temp;
        while (fin >> parameter_type)
        {
            if (parameter_type == "temp_dir:")
                fin >> para.temp_dir;
            else if (parameter_type == "magnitude_factor:")
                fin >> para.magnitude_factor;
            else if (parameter_type == "angle_for_sharp:")
                fin >> para.angle_tol;
            else if (parameter_type == "fit_tolerance:")
                fin >> para.fit_tol;
            else
                fin >> temp;
        }
        fin.close();
    }
}

int CmdReconstructCAD::Run(const char *tri_mesh_file_path, const char *cad_file_path, const char* config_file_path)
{
	// 0. Parse the parameter
    CmdReconstructCADParam para;
    ParseCmdReconstructCADParam(config_file_path, para);

	// 1. read tri mesh
    TriMeshPtr tri_mesh = TriMeshIO().LoadSTL(tri_mesh_file_path);

	// 2. generate quad mesh from tri mesh 
    std::cout << "==============================================\n";
    std::cout << "Generating quad mesh from the triangle mesh...\n";
    
    QuadMeshPtr mesh = QuadMeshGenerator().GenMesh(tri_mesh, para.magnitude_factor, para.preserve_sharp, para.preserve_boundary, para.minimum_cost, para.adaptive_scale, para.angle_tol);
    if (mesh == nullptr)
    {
        printf("Failed to Gen QuadMesh...\n");
        return 1;
    }
    
    const std::string quad_vtk = para.temp_dir + "/quad.vtk";
    int result = QuadMeshIO().SaveVTK(mesh, quad_vtk.c_str());

    result = QuadMeshSpliter().SplitPatch(mesh);
    if (result != 0)
    {
        printf("Failed to Split QuadMesh...\n");
        return 1;
    }

    const std::string patch_vtk = para.temp_dir + "/patch";
    const std::string patch_info = para.temp_dir + "/patch.txt";
    result = QuadMeshIO().SavePatchVTK(mesh, patch_vtk.c_str(), patch_info.c_str());
    if (result != 0)
    {
        printf("Failed to Write QuadMesh Patch...\n");
        return 1;
    }

    // 3. fit NURBS from quad patch 
    const std::string surf = para.temp_dir + "/surf";
    std::cout << "==============================================\n";
    std::cout << "Fitting NURBS...\n";
    MultiPatchFit fit;
    fit.set_fit_tol(para.fit_tol);
    fit.set_degree(3);
    fit.read(patch_info);
    fit.run(surf);
    fit.write(surf);
	
	// 4. write CAD to step 
    const std::string surf_pat = para.temp_dir + "/surf.pat";
    std::cout << "==============================================\n";
    std::cout << "Writing CAD file in step format...\n";
    GenerateShell shell;
    shell.Read(surf_pat.c_str());
    shell.Shell(cad_file_path);

	return 0;
}

} // namespace mft


