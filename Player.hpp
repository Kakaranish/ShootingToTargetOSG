#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <osgViewer/Viewer>

class PointOfViewHandler;

class Player
{
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    PointOfViewHandler *_pointOfViewHandler;

public:
    Player(osg::ref_ptr<osgViewer::Viewer> viewer);

    PointOfViewHandler *getPointOfViewHandler()
    {
        return _pointOfViewHandler;
    }
};

#endif