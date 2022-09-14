//--------------------------------------------------
// Robotic Arm GA
// configComponent.h
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CONFIG_COMPONENT_H
#define CONFIG_COMPONENT_H
#include <atta/component/interface.h>

namespace cmp = atta::component;

struct ConfigComponent final : public cmp::Component {
    /// Number of joints
    int numJoints = 6;
    /// Size of population
    int populationSize = 5;
    /// Mutation
    float mutation = 0.05;

    // Arm parameters
    static constexpr float armLength = 0.4;
    static constexpr float minAngle = -2/3.0f*M_PI;
    static constexpr float maxAngle = 2/3.0f*M_PI;
};
ATTA_REGISTER_COMPONENT(ConfigComponent);
template <>
cmp::ComponentDescription& cmp::TypedComponentRegistry<ConfigComponent>::getDescription();

#endif // CONFIG_COMPONENT_H
