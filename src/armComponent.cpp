//--------------------------------------------------
// Robotic Arm GA
// armComponent.cpp
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "armComponent.h"

namespace cmp = atta::component;

template <>
cmp::ComponentDescription& cmp::TypedComponentRegistry<ArmComponent>::getDescription() {
    static cmp::ComponentDescription desc = {
        "Arm",
        {
            {AttributeType::FLOAT32, offsetof(ArmComponent, fitness), "fitness"},
            {AttributeType::VECTOR_FLOAT32, offsetof(ArmComponent, gene), "gene", -3.14f, 3.14f, 0.01f},
        },
        // Max instances
        1024,
        // Serialize
        {{"gene",
          [](std::ostream& os, void* data) {
              // Do not serialize gene
          }}},
        // Deserialize
        {{"gene",
          [](std::istream& is, void* data) {
              // Do not deserialize gene
          }}},
    };

    return desc;
}
