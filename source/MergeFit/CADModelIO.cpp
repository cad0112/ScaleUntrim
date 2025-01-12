// occt headers
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>

#include "CADModelIO.h"

namespace mft{

CADModelPtr CADModelIO::Load(const char* cad_file_path)
{
	CADModelPtr cad_model = nullptr;
	std::string a = cad_file_path;
	int num = a.find_last_of('.');
	std::string b = a.substr(num + 1);
	if (b == "step" || b == "stp")
	{
		// Create a STEPControl_Reader object
		STEPControl_Reader reader;
		// Read the file and check the status
		IFSelect_ReturnStatus status = reader.ReadFile(cad_file_path);
		if (status == IFSelect_ReturnStatus::IFSelect_RetDone)
		{
			// Transfer the shape from the file to the process
			reader.TransferRoots();
			// Get the shape from the process
			TopoDS_Shape shape = reader.OneShape();
			cad_model = std::make_shared<CADModel>(shape);
		}
	}
	else if (b == "igs" || b == "iges")
	{
		IGESControl_Reader reader;
		// Read the file and check the status
		IFSelect_ReturnStatus status = reader.ReadFile(cad_file_path);
		if (status == IFSelect_ReturnStatus::IFSelect_RetDone)
		{
			// Transfer the shape from the file to the process
			reader.TransferRoots();
			// Get the shape from the process
			TopoDS_Shape shape = reader.OneShape();
			cad_model = std::make_shared<CADModel>(shape);
		}
	}
	else
	{
		std::cout << "Unsupported file formats, please check" << std::endl;
	}	
	return cad_model;
}


}
