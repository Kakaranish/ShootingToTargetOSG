#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <osgViewer/Viewer>
#include "Cannon.hpp"
#include "World.hpp"

class PointOfViewHandler;

class Player
{
private:
    World* _world;
    osg::ref_ptr<osg::Group> _root;
    PointOfViewHandler *_pointOfViewHandler;
    Cannon *_cannon;

public:
    Player(osg::ref_ptr<osgViewer::Viewer> viewer, World* world);

    void shoot();

    PointOfViewHandler *getPointOfViewHandler();
    Cannon *getCannon();
    osg::ref_ptr<osg::Group> getRoot();
};

#endif