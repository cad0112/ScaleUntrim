/**
 * @brief Convert a trimmed CAD modal into an untrimmed NURBS model
 */
#include <iostream>
#include "CmdMergeFit.h"

void print_usage(const char *exe_name)
{
    printf("Usage : %s -i <cad_in.stp> -o <cad_out.stp> -c <setting.config>\n", exe_name);
}

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        print_usage(argv[0]);
        return 1;
    }

    // Parse Input
    std::string cad_in, cad_out, config_file;
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            config_file = std::string(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            cad_in = std::string(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            cad_out = std::string(argv[i + 1]);
        }
    }
    printf("cad_in: %s\ncad_out: %s\nconfig_file: %s\n", cad_in.c_str(), cad_out.c_str(), config_file.c_str());
    if(cad_in.empty() || cad_out.empty() || config_file.empty()){
        print_usage(argv[0]);
        return 1;
    }

    // Run MergeFit
    mft::CmdMergeFit mf;
    mf.set_surface_degree(3);
    mf.run(cad_in, cad_out, config_file);

    return 0;
}
