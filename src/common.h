//--------------------------------------------------
// Robotic Arm GA
// common.h
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef COMMON_H
#define COMMON_H
#include <atta/component/interface.h>

namespace cmp = atta::component;

const cmp::Entity goal(4);
const cmp::Entity config(5);
constexpr float pi = M_PI;

#endif // COMMON_H
