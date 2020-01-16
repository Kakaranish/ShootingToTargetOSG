#include "Player.hpp"
#include "PointOfViewHandler.hpp"

Player::Player(osg::ref_ptr<osgViewer::Viewer> viewer, osg::ref_ptr<osg::Group> root)
{
    _viewer = viewer;
    _root = root;
    _pointOfViewHandler = new PointOfViewHandler(_viewer);
    _pointOfViewHandler->setPlayer(this);
    
    _cannon = new Cannon(_root);
    // _cannon->setRotation(-45);

    _root->addChild(_cannon->getMatrixTransform());
}