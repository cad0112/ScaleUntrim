#include "QuadMeshGenerator.h"

#ifdef WITH_TBB
#include <tbb/tbb.h>
#endif

// std includes
#include <condition_variable>
#include <atomic>
#include <list>
#include <map>
#include <set>
#include <unordered_set>
#include <stdlib.h>
#include <iostream>

// quadriflow includes
#include "adjacent-matrix.hpp"
#include "disajoint-tree.hpp"
#include "field-math.hpp"
#include "hierarchy.hpp"
#include "post-solver.hpp"
#include "serialize.hpp"
#include "config.hpp"
#include "optimizer.hpp"
#include "parametrizer.hpp"

#ifdef WITH_CUDA
#include <cuda_runtime.h>
#endif

using namespace qflow;

namespace mft {
    
QuadMeshPtr QuadMeshGenerator::GenMesh(TriMeshPtr& tri_mesh, double magnitude_factor, int preserve_sharp, int preserve_boundary, int minimum_cost, int adaptive_scale, double angle) 
{
    QuadMeshPtr quad_mesh = std::make_shared<QuadMesh>();
    Parametrizer &field = quad_mesh->Field();

    // load tri mesh
    if (tri_mesh == nullptr) 
        return nullptr;
    field.SetTriMesh(tri_mesh->VMat(), tri_mesh->FMat());

    int t1, t2;
    double faces = magnitude_factor;
    field.flag_preserve_sharp = 0;
    field.flag_preserve_boundary = preserve_boundary;
    field.flag_minimum_cost_flow = 1;
    field.flag_adaptive_scale = 0;

    printf("Initialize...\n");
    t1 = GetCurrentTime64();
    field.Initialize(faces, angle);
    t2 = GetCurrentTime64();
    printf("Use %lf seconds\n", (t2 - t1) * 1e-3);

    if (field.flag_preserve_boundary) {
        printf("Add boundary constrains...\n");
        Hierarchy& mRes = field.hierarchy;
        mRes.clearConstraints();
        std::vector<int> boundaryvertex{};
        for (uint32_t i = 0; i < 3 * mRes.mF.cols(); ++i) {
            if (mRes.mE2E[i] == -1) {
                uint32_t u0 = mRes.mF(i % 3, i / 3);
                uint32_t u1 = mRes.mF((i + 1) % 3, i / 3);
                boundaryvertex.push_back(u0);
                boundaryvertex.push_back(u1);
            }
        }
        for (int i = 0; i < boundaryvertex.size(); ++i) {
            if (boundaryvertex[i] == -1) {
                continue;
            }
            int point1, point2, point3, posi;
            std::vector<int> loop{};
            point1 = boundaryvertex[i];
            boundaryvertex[i] = -1;
            if (i % 2 == 0) {
                point2 = boundaryvertex[i + 1];
                boundaryvertex[i + 1] = -1;
                posi = i + 1;
            } else {
                point2 = boundaryvertex[i - 1];
                boundaryvertex[i - 1] = -1;
                posi = i - 1;
            }
            loop.push_back(point1);
            loop.push_back(point2);
            do {
                for (int j = 0; j < boundaryvertex.size(); ++j) {
                    if (boundaryvertex[j] == -1) continue;
                    if (boundaryvertex[j] == point2 && j != posi) {
                        boundaryvertex[j] = -1;
                        if (j % 2 == 0) {
                            point3 = boundaryvertex[j + 1];
                            boundaryvertex[j + 1] = -1;
                            posi = j + 1;
                        } else {
                            point3 = boundaryvertex[j - 1];
                            boundaryvertex[j - 1] = -1;
                            posi = j - 1;
                        }
                    }
                }
                loop.push_back(point2);
                loop.push_back(point3);
                point2 = point3;
            } while (point3 != point1);
            if (loop.size() < boundaryvertex.size() / 5) {
                if (mRes.mstart.size() == 0) {
                    mRes.mstart.push_back(loop[0]);
                }
            }
            for (int k = 0; k < loop.size(); ++k) {
                if (k % 2 == 1) continue;
                int i0 = loop[k];
                int i1 = loop[k + 1];
                Vector3d p0 = mRes.mV[0].col(i0), p1 = mRes.mV[0].col(i1);
                Vector3d edge = p1 - p0;
                if (edge.squaredNorm() > 0) {
                    edge.normalize();
                    mRes.mCO[0].col(i0) = p0;
                    mRes.mCO[0].col(i1) = p1;
                    mRes.mCQ[0].col(i0) = edge;
                    mRes.mCQw[0][i0] = mRes.mCQw[0][i1] = mRes.mCOw[0][i0] = mRes.mCOw[0][i1] =
                        1.0;
                }
            }
        }
        mRes.propagateConstraints();
    }

    printf("Solve Orientation Field...\n");
    t1 = GetCurrentTime64();

    Optimizer::optimize_orientations(field.hierarchy);
    field.ComputeOrientationSingularities();
    t2 = GetCurrentTime64();
    printf("Use %lf seconds\n", (t2 - t1) * 1e-3);

    if (field.flag_adaptive_scale == 1) {
        printf("Estimate Slop...\n");
        t1 = GetCurrentTime64();
        field.EstimateSlope();
        t2 = GetCurrentTime64();
        printf("Use %lf seconds\n", (t2 - t1) * 1e-3);
    }
    printf("Solve for scale...\n");
    t1 = GetCurrentTime64();
    Optimizer::optimize_scale(field.hierarchy, field.rho, field.flag_adaptive_scale);
    field.flag_adaptive_scale = adaptive_scale;
    t2 = GetCurrentTime64();
    printf("Use %lf seconds\n", (t2 - t1) * 1e-3);

    printf("Solve for position field...\n");
    t1 = GetCurrentTime64();
    Optimizer::optimize_positions(field.hierarchy, field.flag_adaptive_scale);

    field.ComputePositionSingularities();
    t2 = GetCurrentTime64();
    printf("Use %lf seconds\n", (t2 - t1) * 1e-3);
    t1 = GetCurrentTime64();
    printf("Solve index map...\n");

    field.ComputeIndexMap();
    t2 = GetCurrentTime64();
    printf("Indexmap Use %lf seconds\n", (t2 - t1) * 1e-3);

    // char userInput;
    // if (faces == -1) {
    //     std::cout << "The magnitude_factor is initial value " << "\n";
    // }
    // else {
    //     std::cout << "The magnitude_factor is " << faces <<"\n";
    // }
    // std::cout << "please check the quad mesh is correct or not"
    //           << "\n";
    // std::cout << "if it is correct  please cin Y/y"
    //           << "\n";
    // std::cout << "if wrong  please change the magnitude_factor in setting.config "
    //           << "\n";
	// std::cout << "Suggestion: you can try the value from 1 to 2 like 1 , 1.1 , 1.2 , 1.3 "<< " and so on...\n";
    // std::cin >> userInput;
    // if (userInput == 'Y' || userInput == 'y') {
    //     std::cout << "Continuing...." << std::endl;
    // } else {
    //     std::cout << "Stopping the program." << std::endl;
    //     exit(EXIT_SUCCESS);
    // }

    // record quad mesh infomation
	Ver = field.O_compact;
    for (int i = 0; i < field.F_compact.size(); ++i) {
        if (field.F_compact[i][0] == -1)
            continue;
        Fer.push_back(field.F_compact[i]);
    }

    //	field.LoopFace(2);
    return quad_mesh;
}

void QuadMeshGenerator::meshinformation(std::vector<Vector3d> &V_, std::vector<Vector4i> &F_){
    V_ = Ver;
    F_ = Fer;
}

}  // namespace qflow
