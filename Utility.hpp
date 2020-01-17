#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <random>
#include <osg/Geometry>

osg::Vec4f getColor(int r, int g, int b, int alpha = 255);
double degreesToRadians(float degrees);
int randomInt(int start, int end);

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

#endif