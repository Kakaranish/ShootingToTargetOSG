#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <osgViewer/Viewer>
#include "Cannon.hpp"

class PointOfViewHandler;

class Player
{
private:
    osg::ref_ptr<osg::Group> _root;
    PointOfViewHandler *_pointOfViewHandler;
    Cannon *_cannon;

public:
    Player(osg::ref_ptr<osgViewer::Viewer> viewer, osg::ref_ptr<osg::Group> root);

    void shoot();

    PointOfViewHandler *getPointOfViewHandler();
    Cannon *getCannon();
    osg::ref_ptr<osg::Group> getRoot();
};

#endif