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

public:
    Player(osg::ref_ptr<osgViewer::Viewer> viewer);

    PointOfViewHandler *getPointOfViewHandler()
    {
        return _pointOfViewHandler;
    }

    Cannon *getCannon()
    {
        return _cannon;
    }
};

#endif