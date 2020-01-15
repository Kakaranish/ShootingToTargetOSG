#include "Player.hpp"
#include "PointOfViewHandler.hpp"

Player::Player(osg::ref_ptr<osgViewer::Viewer> viewer)
{
    _viewer = viewer;
    _pointOfViewHandler = new PointOfViewHandler(_viewer);
    _pointOfViewHandler->setPlayer(this);
}