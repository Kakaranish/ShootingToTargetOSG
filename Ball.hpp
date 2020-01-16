#ifndef BALL_HPP
#define BALL_HPP

#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <iostream>
#include "Utility.hpp"
#include <math.h>

class Ball : public osg::NodeCallback
{
private:
    const float _gravity;

    osg::ref_ptr<osg::Group> _world;
    osg::ref_ptr<osg::MatrixTransform> _ballMatrixTransform;
    osg::ref_ptr<osg::Geode> _ballGeode;

    osg::Timer _timer;
    float _radius;
    bool _isFalling;
    osg::Vec3f _velocity;

public:
    Ball(osg::ref_ptr<osg::Group> world,
         osg::Vec3f startPosition,
         float radius,
         osg::Vec3f velocity);

    void operator()(osg::Node *node, osg::NodeVisitor *nv);
    osg::ref_ptr<osg::MatrixTransform> getBallMatrixTransform();
};

#endif