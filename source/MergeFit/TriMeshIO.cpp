
#include <StlAPI_Writer.hxx>

#include "TriMeshIO.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

namespace mft
{
	
int TriMeshIO::ConvertStlToObj(const char* stl_file_path, const char* obj_file_path)
{
	TriMeshPtr tri_mesh = LoadSTL(stl_file_path);
	if (tri_mesh == nullptr)
	{
		return 1;
	}

	std::ofstream outfile(obj_file_path);
	if(!outfile.is_open()){
		printf("Failed to open %s.\n", obj_file_path);
		return 1;
	}

	Eigen::MatrixXd &V = tri_mesh->VMat();
	int v_num = (int)(V.cols());
	for (int k = 0; k < v_num; k++)
	{
		outfile << "v" << " " << V(0, k) << " " << V(1, k) << " " << V(2, k) << std::endl;
	}

	Eigen::MatrixXi &F= tri_mesh->FMat();
	int f_num = (int)(F.cols());
	for (int k = 0; k < f_num; k++)
	{
		outfile << "f" << " " << F(0, k)+1 << " " << F(1, k)+1 << " " << F(2, k)+1 << std::endl;
	}
	outfile.close();

	return 0;
}

int TriMeshIO::SaveStl(const CADModelPtr& cad, const char* stl_file_path)
{
	if (cad == nullptr)
		return 1;
	// Write the mesh to the stl file using StlAPI_Writer
	StlAPI_Writer writer;
	writer.Write(cad->Shape(), stl_file_path);
	return 0;
}

namespace{

std::vector<std::string> &str_tokenize(const std::string &s, char delim, std::vector<std::string> &elems, bool include_empty = false) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		if (!item.empty() || include_empty)
			elems.push_back(item);
	return elems;
}

std::vector<std::string> str_tokenize(const std::string &s, char delim, bool include_empty) {
	std::vector<std::string> elems;
	str_tokenize(s, delim, elems, include_empty);
	return elems;
}

uint32_t str_to_uint32_t(const std::string &str) {
	char *end_ptr = nullptr;
	uint32_t result = (uint32_t)strtoul(str.c_str(), &end_ptr, 10);
	if (*end_ptr != '\0')
		throw std::runtime_error("Could not parse unsigned integer \"" + str + "\"");
	return result;
}

}

TriMeshPtr TriMeshIO::LoadOBJ(const char* obj_file_path)
{
	/// Vertex indices used by the OBJ format
	struct obj_vertex {
		uint32_t p = (uint32_t)-1;
		uint32_t n = (uint32_t)-1;
		uint32_t uv = (uint32_t)-1;

		inline obj_vertex() { }

		inline obj_vertex(const std::string &string) {
			std::vector<std::string> tokens = str_tokenize(string, '/', true);

			if (tokens.size() < 1 || tokens.size() > 3)
				throw std::runtime_error("Invalid vertex data: \"" + string + "\"");

			p = str_to_uint32_t(tokens[0]);

#if 0
			if (tokens.size() >= 2 && !tokens[1].empty())
				uv = str_to_uint32_t(tokens[1]);

			if (tokens.size() >= 3 && !tokens[2].empty())
				n = str_to_uint32_t(tokens[2]);
#endif
		}

		inline bool operator==(const obj_vertex &v) const {
			return v.p == p && v.n == n && v.uv == uv;
		}
	};

	/// Hash function for obj_vertex
	struct obj_vertexHash : std::unary_function<obj_vertex, size_t> {
		std::size_t operator()(const obj_vertex &v) const {
			size_t hash = std::hash<uint32_t>()(v.p);
			hash = hash * 37 + std::hash<uint32_t>()(v.uv);
			hash = hash * 37 + std::hash<uint32_t>()(v.n);
			return hash;
		}
	};

	typedef std::unordered_map<obj_vertex, uint32_t, obj_vertexHash> VertexMap;


	std::ifstream is(obj_file_path);

	if(!is.is_open()){
		return nullptr;
	}

	std::vector<Eigen::Vector3d>   positions;
	//std::vector<Vector2d>   texcoords;
	//std::vector<Vector3d>   normals;
	std::vector<uint32_t>   indices;
	std::vector<obj_vertex> vertices;
	VertexMap vertexMap;

	std::string line_str;
	while (std::getline(is, line_str)) {
		std::istringstream line(line_str);

		std::string prefix;
		line >> prefix;

		if (prefix == "v") {
			Eigen::Vector3d p;
			line >> p.x() >> p.y() >> p.z();
			positions.push_back(p);
		}
		else if (prefix == "vt") {
			/*
			Vector2d tc;
			line >> tc.x() >> tc.y();
			texcoords.push_back(tc);
			*/
		}
		else if (prefix == "vn") {
			/*
			Vector3d n;
			line >> n.x() >> n.y() >> n.z();
			normals.push_back(n);
			*/
		}
		else if (prefix == "f") {
			std::string v1, v2, v3, v4;
			line >> v1 >> v2 >> v3 >> v4;
			obj_vertex tri[6];
			int nVertices = 3;

			tri[0] = obj_vertex(v1);
			tri[1] = obj_vertex(v2);
			tri[2] = obj_vertex(v3);

			if (!v4.empty()) {
				/* This is a quad, split into two triangles */
				tri[3] = obj_vertex(v4);
				tri[4] = tri[0];
				tri[5] = tri[2];
				nVertices = 6;
			}
			/* Convert to an indexed vertex list */
			for (int i = 0; i<nVertices; ++i) {
				const obj_vertex &v = tri[i];
				VertexMap::const_iterator it = vertexMap.find(v);
				if (it == vertexMap.end()) {
					vertexMap[v] = (uint32_t)vertices.size();
					indices.push_back((uint32_t)vertices.size());
					vertices.push_back(v);
				}
				else {
					indices.push_back(it->second);
				}
			}
		}
	}

	TriMeshPtr mesh = std::make_shared<TriMesh>();

	mesh->FMat().resize(3, indices.size() / 3);
	memcpy(mesh->FMat().data(), indices.data(), sizeof(uint32_t)*indices.size());

	mesh->VMat().resize(3, vertices.size());
	for (uint32_t i = 0; i < vertices.size(); ++i)
		mesh->VMat().col(i) = positions.at(vertices[i].p - 1);

	return mesh;
}

TriMeshPtr TriMeshIO::LoadSTL(const char* stl_file_path)
{
	std::ifstream file(stl_file_path);
	if (!file.is_open())
	{
		std::cout << "failed to open file" << std::endl;
		return nullptr;
	}

	std::vector<double> vertex_x;
	std::vector<double> vertex_y;
	std::vector<double> vertex_z;
	int j = 0;
	int face = 0;
	double tolerance = 1e-8;
	int flag_3 = 0;
	int flag = 0;
	std::vector<std::vector<int>> group;
	std::vector<int> group_row;
	std::string line;
	while (getline(file, line))
	{
		if (line.find("facet normal") != std::string::npos)
		{
			face = face + 1;
		}
		if (line.find("vertex") != std::string::npos)
		{
			std::stringstream ss(line);
			std::string vertex;
			double x, y, z;
			ss >> vertex >> x >> y >> z;
			for (int m = 0; m <= j; m++)
			{
				if (m == j || j == 0)
				{
					vertex_x.push_back(x);
					vertex_y.push_back(y);
					vertex_z.push_back(z);
					group_row.push_back(j);
					j = j + 1;
					break;
				}
				if ((abs(x - vertex_x[m]) <= tolerance) && (abs(y - vertex_y[m]) <= tolerance) && (abs(z - vertex_z[m]) <= tolerance))
				{
					group_row.push_back(m);
					break;
				}
			}
			flag_3++;
			if (flag_3 == 3)
			{
				flag_3 = 0;
				flag++;
				group.push_back(group_row);
				group_row.clear();
			}
		}
	}
	
	std::cout << "vertex = " << j << std::endl;
	std::cout << "face = " << flag << std::endl;

	TriMeshPtr mesh = std::make_shared<TriMesh>();

	mesh->VMat().resize(3, j);
	for (int k = 0; k < (j); k++)
	{
		mesh->VMat()(0, k) = vertex_x[k];
		mesh->VMat()(1, k) = vertex_y[k];
		mesh->VMat()(2, k) = vertex_z[k];
	}

	mesh->FMat().resize(3, flag);
	for (int k = 0; k < flag; k++)
	{
		mesh->FMat()(0, k) = group[k][0];
		mesh->FMat()(1, k) = group[k][1];
		mesh->FMat()(2, k) = group[k][2];
	}

	return mesh;
}


} // namespace mft


