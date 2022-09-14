//--------------------------------------------------
// Robotic Arm GA
// projectScript.cpp
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"
#include "armComponent.h"
#include "configComponent.h"
#include "imgui.h"
#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/name.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>

void Project::onLoad() { _running = false; }

void Project::onStart() {
    _running = true;
    init();
}

void Project::onStop() {
    _running = false;
    clear();
}

void Project::onUpdateBefore(float dt) {
    for (cmp::Entity arm : _arms)
        updateArm(arm);
}

void Project::onUIRender() {
    ImGui::Begin("Configuration");
    {
        ImGui::Text("Population size");
        if (ImGui::DragInt("##Population size", &config.get<ConfigComponent>()->populationSize, 1, 1, 50) && _running) {
            clear();
            init();
        }
        ImGui::Text("Number of joints");
        if (ImGui::DragInt("##Number of joints", &config.get<ConfigComponent>()->numJoints, 1, 0, 10) && _running) {
            clear();
            init();
        }
    }
    ImGui::End();
}

void Project::init() {
    const int populationSize = config.get<ConfigComponent>()->populationSize;
    for (int i = 0; i < populationSize; i++)
        _arms.push_back(createArm());
}

void Project::clear() {
    // Delete all joints
    for (cmp::Entity arm : _arms)
        cmp::deleteEntity(arm);
    _arms.clear();
}

cmp::Entity Project::createArm() {
    const int numJoints = config.get<ConfigComponent>()->numJoints;

    // Create root
    cmp::Entity root = cmp::createEntity();
    root.add<cmp::Transform>();
    root.add<cmp::Name>()->set("Robotic arm");
    root.add<ArmComponent>();
    root.add<cmp::Relationship>();

    // Create base
    cmp::Entity base = cmp::createEntity();
    base.add<cmp::Transform>();
    base.add<cmp::Mesh>()->set("base.stl");
    base.add<cmp::Name>()->set("Base");
    base.add<cmp::Material>()->set("Orange");
    base.add<cmp::Relationship>()->setParent(root, base);

    cmp::Entity parent = root;
    for (int i = 0; i < numJoints; i++) {
        // Create joint
        cmp::Entity joint = cmp::createEntity();
        joint.add<cmp::Relationship>()->setParent(parent, joint);
        joint.add<cmp::Name>()->set("Joint " + std::to_string(i));
        if (i == 0)
            joint.add<cmp::Transform>()->position.z = 0.05;
        else {
            joint.add<cmp::Transform>()->position[i == 1 ? 2 : 0] = ConfigComponent::armLength;
            joint.get<cmp::Transform>()->orientation.fromEuler({i > 1 ? pi/2 : 0.0f, i == 1 ? pi/2 : 0.0f, 0.0f});
        }
        joint.add<cmp::Mesh>()->set("joint.stl");
        joint.add<cmp::Material>()->set("Gray");

        // Create arm
        cmp::Entity arm = cmp::createEntity();
        arm.add<cmp::Relationship>()->setParent(joint, arm);
        arm.add<cmp::Name>()->set("Arm " + std::to_string(i));
        arm.add<cmp::Transform>()->position[i == 0 ? 2 : 0] = ConfigComponent::armLength / 2;
        if (i != 0)
            arm.get<cmp::Transform>()->orientation.rotateAroundAxis(atta::vec3(0, 1, 0), M_PI / 2);
        arm.add<cmp::Mesh>()->set("arm.stl");
        arm.add<cmp::Material>()->set("Orange");

        parent = joint;
    }

    // Hand
    cmp::Entity hand = cmp::createEntity();
    hand.add<cmp::Relationship>()->setParent(parent, hand);
    hand.add<cmp::Name>()->set("Hand");
    hand.add<cmp::Transform>()->position.x = ConfigComponent::armLength;
    hand.get<cmp::Transform>()->scale = atta::vec3(0.15f);
    hand.add<cmp::Mesh>()->set("meshes/sphere.obj");
    hand.add<cmp::Material>()->set("Red");

    return root;
}

void Project::updateArm(cmp::Entity arm) {
    const int numJoints = config.get<ConfigComponent>()->numJoints;
    const auto gene = arm.get<ArmComponent>()->gene;

    cmp::Entity parent = arm;
    for (int i = 0; i < numJoints; i++) {
        cmp::Entity joint = parent.get<cmp::Relationship>()->getChildren()[1];

        atta::quat ori{};
        ori.rotateAroundAxis(atta::vec3(0, 0, 1), gene[i]);
        ori.rotateAroundAxis(atta::vec3(1, 0, 0), i > 1 ? pi/2 : 0.0f);
        ori.rotateAroundAxis(atta::vec3(0, 1, 0), i == 1 ? pi/2 : 0.0f);
        joint.get<cmp::Transform>()->orientation = ori;
        parent = joint;
    }
}
