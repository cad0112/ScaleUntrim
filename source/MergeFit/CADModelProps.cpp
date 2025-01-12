// occt headers
#include <TopExp_Explorer.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>

#include "CADModelProps.h"

namespace mft{

double CADModelProps::ComputeArea(CADModelPtr& cad)
{
	double total_area = 0.0;
	if (cad != nullptr)
	{
		// Iterate over all faces in the shape
		for (TopExp_Explorer explorer(cad->Shape(), TopAbs_FACE); explorer.More(); explorer.Next())
		{
			const TopoDS_Face &face = TopoDS::Face(explorer.Current());

			// Create a GProp_GProps object and pass it the surface
			GProp_GProps props;
			BRepGProp::SurfaceProperties(face, props);

			// Get the surface area of the face
			double area = props.Mass();

			// Add the area of this face to the total area
			total_area += area;
		}
	}
	return total_area;
}


}
