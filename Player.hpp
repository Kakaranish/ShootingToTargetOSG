#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <osgViewer/Viewer>
#include "Cannon.hpp"

class PointOfViewHandler;

class Player
{
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    PointOfViewHandler *_pointOfViewHandler;
    Cannon *_cannon;
    osg::ref_ptr<osg::Group> _root;

public:
    Player(osg::ref_ptr<osgViewer::Viewer> viewer, osg::ref_ptr<osg::Group> root);

    PointOfViewHandler *getPointOfViewHandler()
    {
        return _pointOfViewHandler;
    }

    void shoot(){
        Ball* ball = _cannon->getBall();
        _root->addChild(ball->ballMatrix);
    }


    void rotateCannon(float degrees)
    {
        _cannon->rotate(degrees);
    }

    Cannon *getCannon()
    {
        return _cannon;
    }
};

#endif