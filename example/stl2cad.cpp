/**
 * @brief Reconstruct CAD model from STL
 */
#include <iostream>
#include <string.h>
#include "CmdReconstructCAD.h"

void print_usage(const char *exe_name)
{
    printf("Usage : %s -i <tri_in.stl> -o <cad_out.stp> [-c <setting.config>]\n", exe_name);
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        print_usage(argv[0]);
        return 1;
    }

    // Parse Input
    char *tri_in = NULL;
    char *cad_out = NULL;
    char *config_file = NULL;
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            tri_in = argv[i + 1];
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            cad_out = argv[i + 1];
        }else if (strcmp(argv[i], "-c") == 0)
        {
            config_file = argv[i + 1];
        }
    }
    printf("tri_in: %s\n cad_out: %s\n config_file: %s\n", tri_in, cad_out, config_file);
    if (tri_in == NULL || cad_out == NULL)
    {
        print_usage(argv[0]);
        return 1;
    }

    // Run CAD Reconstruct
    if(mft::CmdReconstructCAD::Run(tri_in, cad_out, config_file) == 0){
        std::cout << " Reconstruct CAD OK" << std::endl;
    }else{
        std::cout << " Reconstruct CAD Failed" << std::endl;
    }

    return 0;
}
