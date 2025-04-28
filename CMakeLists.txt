cmake_minimum_required(VERSION 3.14)
project(robotic-arm-ga)

find_package(atta 0.3.9 REQUIRED)

# Custom components
atta_add_target(config_component "src/configComponent.cpp")
atta_add_target(arm_component "src/armComponent.cpp")

# Project script
atta_add_target(project_script "src/projectScript.cpp")
target_link_libraries(project_script PRIVATE config_component arm_component atta::imgui)
