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

void Project::onUpdateBefore(float dt) { updateArm(armPrototype); }

void Project::onUIRender() {
    ImGui::Begin("Configuration");
    int number = 0;
    if (!_running) {
        ImGui::Text("Number of joints");
        ImGui::DragInt("##Number of joints", &config.get<ConfigComponent>()->numJoints, 1, 0, 10);
    }
    ImGui::End();
}

void Project::init() { createArm(armPrototype); }

void Project::clear() {
    // Delete all joints
    for (cmp::Entity entity : _toDelete)
        cmp::deleteEntity(entity);
    _toDelete.clear();
}

void Project::createArm(cmp::Entity arm) {
    const int numJoints = config.get<ConfigComponent>()->numJoints;

    cmp::Entity parent = arm;
    for (int i = 0; i < numJoints; i++) {
        // Create joint
        cmp::Entity joint = cmp::createEntity();
        joint.add<cmp::Relationship>()->setParent(parent, joint);
        joint.add<cmp::Name>()->set("Joint " + std::to_string(i));
        if (i == 0)
            joint.add<cmp::Transform>()->position.z = 0.05;
        else {
            if (i % 2) {
                joint.add<cmp::Transform>()->position.z = ConfigComponent::armLength;
                joint.get<cmp::Transform>()->orientation.rotateAroundAxis(atta::vec3(0, 1, 0), M_PI / 2);
            } else {
                joint.add<cmp::Transform>()->position.x = ConfigComponent::armLength;
                joint.get<cmp::Transform>()->orientation.rotateAroundAxis(atta::vec3(0, 1, 0), -M_PI / 2);
            }
        }
        joint.add<cmp::Mesh>()->set("joint.stl");
        joint.add<cmp::Material>()->set("Gray");

        // Create arm
        cmp::Entity arm = cmp::createEntity();
        arm.add<cmp::Relationship>()->setParent(joint, arm);
        arm.add<cmp::Name>()->set("Arm " + std::to_string(i));
        if (i % 2) {
            arm.add<cmp::Transform>()->position.x = ConfigComponent::armLength / 2;
            arm.get<cmp::Transform>()->orientation.rotateAroundAxis(atta::vec3(0, 1, 0), M_PI / 2);
        } else
            arm.add<cmp::Transform>()->position.z = ConfigComponent::armLength / 2;

        arm.add<cmp::Mesh>()->set("arm.stl");
        arm.add<cmp::Material>()->set("Orange");

        // Hand
        if(i == numJoints-1)
        {
            cmp::Entity hand = cmp::createEntity();
            hand.add<cmp::Relationship>()->setParent(joint, hand);
            hand.add<cmp::Name>()->set("Hand");
            if (i % 2)
                hand.add<cmp::Transform>()->position.x = ConfigComponent::armLength;
            else
                hand.add<cmp::Transform>()->position.z = ConfigComponent::armLength;
            hand.get<cmp::Transform>()->scale = atta::vec3(0.15f);


            hand.add<cmp::Mesh>()->set("meshes/sphere.obj");
            hand.add<cmp::Material>()->set("Red");
        }

        if (i == 0) // When parent entity is deleted, children are deleted
            _toDelete.push_back(joint);
        parent = joint;
    }
}

void Project::updateArm(cmp::Entity arm) {
    const int numJoints = config.get<ConfigComponent>()->numJoints;
    const auto gene = arm.get<ArmComponent>()->gene;

    cmp::Entity parent = arm;
    for (int i = 0; i < numJoints; i++) {
        cmp::Entity joint = parent.get<cmp::Relationship>()->getChildren()[1];

        atta::quat ori{};
        ori.rotateAroundAxis(atta::vec3(0, 0, 1), gene[i]);
        if (i != 0)
            ori.rotateAroundAxis(atta::vec3(0, 1, 0), M_PI / 2 * (i % 2 ? 1 : -1));

        joint.get<cmp::Transform>()->orientation = ori;
        parent = joint;
    }
}
