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
         osg::Vec3f velocity) : _gravity(-0.01),
                                _world(world),
                                _radius(radius),
                                _velocity(velocity)

    {
        _isFalling = true;

        osg::ref_ptr<osg::ShapeDrawable> ball = new osg::ShapeDrawable(
            new osg::Sphere(startPosition, radius));
        ball->setColor(getColor(112, 255, 0));
        _ballGeode = new osg::Geode;
        _ballGeode->addDrawable(ball.get());

        _ballMatrixTransform = new osg::MatrixTransform;
        _ballMatrixTransform->addChild(_ballGeode);
        _ballMatrixTransform->setUpdateCallback(this);
        _world->addChild(_ballMatrixTransform);
    }

    void operator()(osg::Node *node, osg::NodeVisitor *nv)
    {
        if (_isFalling == false)
        {
            if (_timer.time_m() > 2 * 1000)
            {
                _ballMatrixTransform->removeChild(_ballGeode);
                _world->removeChild(_ballMatrixTransform);
                _ballMatrixTransform->removeUpdateCallback(this);
            }
            
            return;
        }

        _velocity += osg::Vec3f(0, 0, _gravity);

        if (_ballMatrixTransform->getBound().center().z() < _radius)
        {
            osg::Vec3f ballPosition = _ballMatrixTransform->getMatrix().getTrans();
            osg::Vec3f newBallPosition =
                ballPosition + osg::Vec3f(0, 0, _radius - _ballMatrixTransform->getBound().center().z());
            _ballMatrixTransform->setMatrix(osg::Matrix::translate(newBallPosition));

            _isFalling = false;
            _timer.setStartTick();
            return;
        }

        osg::Vec3f newPosition = _ballMatrixTransform->getMatrix().getTrans() + _velocity;
        _ballMatrixTransform->setMatrix(osg::Matrix::translate(newPosition));
    }

    osg::ref_ptr<osg::MatrixTransform> getBallMatrixTransform()
    {
        return _ballMatrixTransform;
    }
};

#endif