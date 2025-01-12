/**
 * @brief Gen STL from CAD model
 */
#include <iostream>
#include <string.h>
#include "CmdTesselateCAD.h"

void print_usage(const char *exe_name)
{
    printf("Usage : %s -i <cad_in.stp> -o <tri_out.stl> -c <setting.config>\n", exe_name);
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        print_usage(argv[0]);
        return 1;
    }

    // Parse Input
    char *cad_in = NULL;
    char *tri_out = NULL;
    char *config_file = NULL;
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            cad_in = argv[i + 1];
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            tri_out = argv[i + 1];
        }else if (strcmp(argv[i], "-c") == 0)
        {
            config_file = argv[i + 1];
        }
    }
    printf("cad_in: %s\ntri_out: %s\nconfig_file: %s\n", cad_in, tri_out, config_file);
    if (cad_in == NULL || tri_out == NULL)
    {
        print_usage(argv[0]);
        return 1;
    }

    // Run CAD Tesselation
    if(mft::CmdTesselateCAD::Run(cad_in, tri_out, config_file) == 0){
        std::cout << " Tesselate CAD OK" << std::endl;
    }else{
        std::cout << " Tesselate CAD Failed" << std::endl;
    }

    return 0;
}
