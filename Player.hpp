#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <osgViewer/Viewer>
#include "Cannon.hpp"

class PointOfViewHandler;

class Player
{
    const float _verticalMovement;
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    PointOfViewHandler *_pointOfViewHandler;
    Cannon *_cannon;
    osg::ref_ptr<osg::Group> _root;

public:
    osg::Timer timer;

    Player(osg::ref_ptr<osgViewer::Viewer> viewer, osg::ref_ptr<osg::Group> root);

    PointOfViewHandler *getPointOfViewHandler()
    {
        return _pointOfViewHandler;
    }

    void shoot()
    {
        Ball *ball = _cannon->createBall();
        std::cout << "in player: " << ball->getBallMatrixTransform() << std::endl;
        _root->addChild(ball->getBallMatrixTransform());
    }

    void showElapsedTime()
    {
        std::cout << timer.time_s() << std::endl;
        timer.setStartTick();
    }

    Cannon *getCannon()
    {
        return _cannon;
    }
};

#endif