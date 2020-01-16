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
public:
    osg::ref_ptr<osg::MatrixTransform> ballMatrix;
    osg::ref_ptr<osg::Geode> geode;
    float speed;
    float radius;
    bool finished;
    float xSpeed;
    float angle;
    float ySpeed;
    float zSpeed;
    osg::ref_ptr<osg::Group> _world;
    osg::Timer timer;

    Ball(osg::ref_ptr<osg::Group> world, osg::Vec3f position = osg::Vec3f(0, 0, 6))
    {
        xSpeed = 0.03;
        finished = false;
        speed = 0.4;
        radius = 0.4;
        _world = world;

        // ---------------------------------------------

        angle = degreesToRadians(45);
        zSpeed = sin(angle) * 0.5;
        ySpeed = cos(angle) * 0.5;

        osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(position, radius);
        // ball = new osg::ShapeDrawable(sphere);
        osg::ref_ptr<osg::ShapeDrawable> ball = new osg::ShapeDrawable(sphere);
        ball->setColor(getColor(112, 255, 0));
        geode = new osg::Geode;
        geode->addDrawable(ball.get());

        ballMatrix = new osg::MatrixTransform;
        ballMatrix->addChild(geode);
        ballMatrix->setUpdateCallback(this);
        _world->addChild(ballMatrix);
    }

    void operator()(osg::Node *node, osg::NodeVisitor *nv)
    {
        if (finished)
        {
            if (timer.time_m() > 2 * 1000)
            {
                std::cout << "in ball: " << ballMatrix << std::endl;
                ballMatrix->removeChild(geode);
                _world->removeChild(ballMatrix);
                ballMatrix->removeUpdateCallback(this);
            }
            return;
        }

        float gravity = -0.01;

        zSpeed += gravity;

        // std::cout << ballMatrix->getBound().center().x() << ", " << ballMatrix->getBound().center().y() << ", " << ballMatrix->getBound().center().z() << std::endl;

        if (ballMatrix->getBound().center().z() < radius)
        {
            osg::Vec3f pos = ballMatrix->getMatrix().getTrans();
            float diff = -ballMatrix->getBound().center().z();
            pos += osg::Vec3f(0, 0, diff + radius);
            ballMatrix->setMatrix(osg::Matrix::translate(pos));
            finished = true;

            timer.setStartTick();
            return;
        }

        osg::Vec3f pos = ballMatrix->getMatrix().getTrans();
        osg::Vec3f newPos = pos + osg::Vec3f(0, ySpeed, zSpeed);
        // std::cout << newPos.x() << ", " << newPos.y() << ", " << newPos.z() << std::endl;
        ballMatrix->setMatrix(osg::Matrix::translate(newPos));
    }
};

#endif