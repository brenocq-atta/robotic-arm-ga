//--------------------------------------------------
// Robotic Arm GA
// armComponent.h
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ARM_COMPONENT_H
#define ARM_COMPONENT_H
#include <atta/component/interface.h>

namespace cmp = atta::component;

struct ArmComponent final : public cmp::Component {
    /// Arm fitness
    /** Distance from goal **/
    float fitness;

    // Arm gene
    /** Angle of each joint **/
    float gene[20];
};
ATTA_REGISTER_COMPONENT(ArmComponent);
template <>
cmp::ComponentDescription& cmp::TypedComponentRegistry<ArmComponent>::getDescription();

#endif // ARM_COMPONENT_H
