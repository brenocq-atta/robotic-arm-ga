cmake_minimum_required(VERSION 3.12)
project(robotic-arm-ga VERSION 1.0.0 LANGUAGES CXX)
find_package(atta 0.3.2 REQUIRED)

atta_add_target(config_component "src/configComponent.cpp")
atta_add_target(arm_component "src/armComponent.cpp")

atta_add_target(project_script "src/projectScript.cpp")
target_link_libraries(project_script PRIVATE config_component arm_component)
