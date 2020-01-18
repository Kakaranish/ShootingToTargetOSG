#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <random>
#include <osg/Geode>
#include <osg/ShapeDrawable>

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

osg::Vec4f getColor(int r, int g, int b, int alpha = 255);

double degreesToRadians(float degrees);

int randomInt(int start, int end);

osg::ref_ptr<osg::Geode> createCoordinateSystem(
    float distanceFromZero,
    float sphereRadius = 1.f);

osg::ref_ptr<osg::Geode> createBall(osg::Vec3f position, float radius);

#endif