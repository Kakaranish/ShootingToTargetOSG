#ifndef BALL_HPP
#define BALL_HPP

#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <iostream>
#include <math.h>
#include "Utility.hpp"
#include "World.hpp"

class Ball : public osg::NodeCallback
{
private:
    const float _gravity;

    World *_world;
    osg::ref_ptr<osg::MatrixTransform> _ballMatrixTransform;
    osg::ref_ptr<osg::Geode> _ballGeode;

    osg::Timer _timer;
    float _radius;
    bool _isFalling;
    osg::Vec3f _velocity;

    osg::Vec3f getCurrentPosition();
    osg::Vec3f calculateNextPosition();
    ShootingTarget *getCollidingShootingTarget();
    bool collidesWithShootingTarget(ShootingTarget *shootingTarget);

public:
    Ball(World *world,
         osg::Vec3f startPosition,
         float radius,
         osg::Vec3f velocity);

    void operator()(osg::Node *node, osg::NodeVisitor *nv);
    osg::ref_ptr<osg::MatrixTransform> getBallMatrixTransform();
};

#endif