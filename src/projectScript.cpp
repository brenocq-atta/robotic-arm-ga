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
    static float time = 0;
    time += dt;

    for (cmp::Entity arm : _arms)
        updateArmJoints(arm);

    if (time > 1.0f)
        evolve();
}

void Project::onUIRender() {
    ImGui::Begin("Configuration");
    {
        ImGui::Text("Population size");
        if (ImGui::DragInt("##Population size", &config.get<ConfigComponent>()->populationSize, 1, 2, 50) && _running) {
            clear();
            init();
        }
        ImGui::Text("Number of joints");
        if (ImGui::DragInt("##Number of joints", &config.get<ConfigComponent>()->numJoints, 1, 1, 20) && _running) {
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
    auto& gene = root.add<ArmComponent>()->gene;
    for (int i = 0; i < numJoints; i++) {
        float r = rand() / float(RAND_MAX);
        gene[i] = r * (ConfigComponent::maxAngle - ConfigComponent::minAngle) + ConfigComponent::minAngle;
    }

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
            joint.get<cmp::Transform>()->orientation.fromEuler({i > 1 ? pi / 2 : 0.0f, i == 1 ? pi / 2 : 0.0f, 0.0f});
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
    hand.add<cmp::Transform>()->position[numJoints > 1 ? 0 : 2] = ConfigComponent::armLength;
    hand.get<cmp::Transform>()->scale = atta::vec3(0.15f);
    hand.add<cmp::Mesh>()->set("meshes/sphere.obj");
    hand.add<cmp::Material>()->set("Red");

    return root;
}

void Project::updateArmJoints(cmp::Entity arm) {
    const int numJoints = config.get<ConfigComponent>()->numJoints;
    const auto gene = arm.get<ArmComponent>()->gene;

    cmp::Entity parent = arm;
    for (int i = 0; i < numJoints; i++) {
        cmp::Entity joint = parent.get<cmp::Relationship>()->getChildren()[1]; // Get joint

        atta::quat ori{};
        ori.rotateAroundAxis(atta::vec3(0, 0, 1), gene[i]);
        if (i > 1)
            ori.rotateAroundAxis(atta::vec3(1, 0, 0), pi / 2);
        else if (i == 1)
            ori.rotateAroundAxis(atta::vec3(0, 1, 0), pi / 2);
        joint.get<cmp::Transform>()->orientation = ori;
        parent = joint;
    }
}

void Project::evolve() {
    const int numJoints = config.get<ConfigComponent>()->numJoints;
    const float mutation = config.get<ConfigComponent>()->mutation;
    std::vector<float> fitness;

    for (cmp::Entity arm : _arms) {
        // Get hand
        cmp::Entity hand = arm;
        for (int i = 0; i < numJoints + 1; i++)
            hand = hand.get<cmp::Relationship>()->getChildren()[1];
        // Calculate dist to goal
        atta::vec3 dist = goal.get<cmp::Transform>()->position - hand.get<cmp::Transform>()->getWorldTransform(hand).getPosition();
        fitness.push_back(dist.length());
    }

    // Find best individual
    int bestIdx = 0;
    for (int i = 1; i < fitness.size(); i++)
        if (fitness[i] < fitness[bestIdx])
            bestIdx = i;
    auto& geneBest = _arms[bestIdx].get<ArmComponent>()->gene;

    // Generate new population
    for (int a = 0; a < _arms.size(); a++) {
        if (a == bestIdx)
            continue;
        auto& gene = _arms[a].get<ArmComponent>()->gene;
        for (int j = 0; j < numJoints; j++) {
            // Cross genes
            gene[j] = (gene[j] + geneBest[j]) / 2;
            // Mutate
            gene[j] += (rand() / float(RAND_MAX) - 0.5f) * mutation;
        }
    }
}
