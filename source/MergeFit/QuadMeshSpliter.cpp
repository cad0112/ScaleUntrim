#include "QuadMeshSpliter.h"
#include "parametrizer.hpp"
#include "optimizer.hpp"

using namespace qflow;

namespace mft {

int QuadMeshSpliter::SplitPatch(QuadMeshPtr& mesh)
{
    if (mesh == nullptr)
        return 1;
    printf("Spliting Patches...\n");
    Parametrizer &field = mesh->Field();
    Optimizer::extract_patch(field.hierarchy, field.F_new, field.V_new, field.N_new, field.Q_new,
                             field.Vset_new, field.O_compact_new, field.F_compact_new,
                             field.V2E_compact_new, field.E2E_compact_new, field.mScale_new,
                             field.diffs_new, field.diff_count_new, field.patch_compact,
                             field.sharp_o_new, field.compact_sharp_constraints_new,
                             field.boundary_o_new, field.compact_boundary_constraints_new);
    return 0;
}

}
