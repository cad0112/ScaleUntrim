#include <BRepTools.hxx>
#include <StlAPI_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Poly_Triangulation.hxx>
#include <BRep_Tool.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRepBndLib.hxx>
#include <BRepBuilderAPI.hxx>
#include <Bnd_Box.hxx>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <IGESControl_Reader.hxx>

#include "CADModelIO.h"
#include "TriMeshIO.h"
#include "CADModelTesselator.h"
#include "CADModelProps.h"

double compute_area(const char *in_file)
{
	CADModelPtr cad = mft::CADModelIO().Load(in_file);
	return mft::CADModelProps().ComputeArea(cad);
}

void triangular(const char* input_file, const char* output_file, double ratio = 0.0001)
{
	CADModelPtr cad = mft::CADModelIO().Load(input_file);
	if (cad != nullptr)
	{
		mft::CADModelTesselator().GenTriMesh(cad, ratio);
		mft::TriMeshIO().SaveStl(cad, output_file);
	}
};

void convert(const char* conv_in, const char* conv_out)
{
	mft::TriMeshIO().ConvertStlToObj(conv_in, conv_out);
}
