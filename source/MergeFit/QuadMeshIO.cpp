#include "QuadMeshIO.h"
#include "parametrizer.hpp"

#include <fstream>

using namespace qflow;

namespace mft {

int QuadMeshIO::SaveVTK(QuadMeshPtr &mesh, const char *output_path)
{
    if (mesh == nullptr || output_path == NULL)
        return 1;
    printf("Writing the file %s...\n", output_path);

    Parametrizer &field = mesh->Field();
    std::ofstream os(output_path);
    os << "# vtk DataFile Version 2.0"
       << "\n";
    os << "name, Created by ScaleUntrim "
       << "\n";
    os << "ASCII"
       << "\n";
    os << "DATASET UNSTRUCTURED_GRID"
       << "\n";
    os << "POINTS"
       << " " << field.O_compact.size() << " "
       << "double"
       << "\n";

    for (int i = 0; i < field.O_compact.size(); ++i) {
        auto t = field.O_compact[i] * field.normalize_scale + field.normalize_offset;

        os << t[0] << " " << t[1] << " " << t[2] << "\n";
    }
    os << "\n";

    std::vector<int> quad{};
    for (int i = 0; i < field.F_compact.size(); ++i) {
        if (field.F_compact[i][0] != -1) {
            quad.push_back(field.F_compact[i][0]);
            quad.push_back(field.F_compact[i][1]);
            quad.push_back(field.F_compact[i][2]);
            quad.push_back(field.F_compact[i][3]);
        }
    }
    int a = quad.size() / 4;
    os << "CELLS"
       << " " << a << " " << 5 * a << "\n";
    for (int i = 0; i < a; ++i) {
        os << 4 << " " << quad[4 * i] << " " << quad[4 * i + 1] << " " << quad[4 * i + 2] << " "
           << quad[4 * i + 3] << "\n";
    }
    os << "\n";
    os << "CELL_TYPES"
       << " " << a << "\n";
    for (int i = 0; i < a; ++i) {
        os << 9 << "\n";
    }
    os.close();

    return 0;
}

int QuadMeshIO::SaveOBJ(QuadMeshPtr& mesh, const char* output_path)
{
    Parametrizer &field = mesh->Field();
    std::ofstream os(output_path);
    for (int i = 0; i < field.O_compact.size(); ++i)
    {
        auto t = field.O_compact[i] * field.normalize_scale + field.normalize_offset;
        os << "v " << t[0] << " " << t[1] << " " << t[2] << "\n";
    }
    for (int i = 0; i < field.F_compact.size(); ++i)
    {
        os << "f " << field.F_compact[i][0] + 1 << " " << field.F_compact[i][1] + 1
           << " " << field.F_compact[i][2] + 1 << " " << field.F_compact[i][3] + 1
           << "\n";
    }
    os.close();
}

int QuadMeshIO::SavePatchVTK(QuadMeshPtr& mesh, const char* output_patch_file, const char* output_patch_information)
{
    if (mesh == nullptr || output_patch_file == NULL || output_patch_information == NULL)
        return 1;
    printf("Writing Patches...\n");
    Parametrizer &field = mesh->Field();
    field.Outputpatch(output_patch_file, output_patch_information);
    return 0;
}

}
