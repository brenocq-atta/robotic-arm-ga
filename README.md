<!--
title: Robotic Arm GA
description: Robotic arm control with any number of joints using genetic algorithms
image: https://storage.googleapis.com/atta-images/docs/robotic-arm-ga/robotic-arm-ga.gif
build: https://storage.googleapis.com/atta-repos/robotic-arm-ga
-->
## Introduction
This project aims to control a robot arm with an arbitrary number of joints using genetic algorithms.
Today it is possible to change the number of joints and the accuracy of movement.

## The evolution
A new population of 10 individuals is created each generation. The individual has a gene with the rotation for each joint. To decide which individual will be chosen to move the arm, the distance between the arm end and the goal is calculated and the individual that minimizes this distance is selected. After selecting the individual, new individuals are generated from the previous winner.

## Results
Today, the arm can go from the current position to the desired position with any number of joints, but no collisions are being considered, resulting in impossible movements.

## License
This code is released under MIT license.
