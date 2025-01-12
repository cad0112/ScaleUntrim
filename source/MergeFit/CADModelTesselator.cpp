// occt headers
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

#include "CADModelTesselator.h"

namespace mft{

int CADModelTesselator::GenTriMesh(CADModelPtr& cad, const double& linear_deflection, const double& angular_deflection)
{
	if (cad == nullptr)
		return 1;
	// Generate the mesh using BRepMesh_IncrementalMesh
	BRepMesh_IncrementalMesh mesher(cad->Shape(), linear_deflection, Standard_False, angular_deflection, Standard_True);
	mesher.Perform();
	return 0;
}

int CADModelTesselator::GenTriMesh(CADModelPtr& cad, const double& ratio)
{
	if (cad == nullptr)
		return 1;

	Bnd_Box box;
	BRepBndLib::Add(cad->Shape(), box);
	Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
	box.Get(xmin, ymin, zmin, xmax, ymax, zmax);
	Standard_Real dx = xmax - xmin;
	Standard_Real dy = ymax - ymin;
	Standard_Real dz = zmax - zmin;
	Standard_Real diagonal = sqrt(dx * dx + dy * dy + dz * dz);
	// std::cout << "diagonal = " << diagonal << std::endl;

	return GenTriMesh(cad, diagonal * ratio, 50.0);
}

}
