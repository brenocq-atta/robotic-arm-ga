//--------------------------------------------------
// Robotic Arm GA
// configComponent.cpp
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "configComponent.h"

namespace cmp = atta::component;

template <>
cmp::ComponentDescription& cmp::TypedComponentRegistry<ConfigComponent>::getDescription() {
    static cmp::ComponentDescription desc = {"Config",
                                             {
                                                 {AttributeType::INT32, offsetof(ConfigComponent, numJoints), "numJoints"},
                                                 {AttributeType::FLOAT32, offsetof(ConfigComponent, mutation), "mutation"},
                                             },
                                             // Max instances
                                             1};

    return desc;
}
