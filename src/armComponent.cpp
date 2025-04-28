//--------------------------------------------------
// Robotic Arm GA
// armComponent.cpp
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "armComponent.h"

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
    };

    return desc;
}
