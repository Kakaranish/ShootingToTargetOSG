#include "Player.hpp"
#include "PointOfViewHandler.hpp"

Player::Player(osg::ref_ptr<osgViewer::Viewer> viewer, osg::ref_ptr<osg::Group> root) : _verticalMovement(0.5)
{
    _viewer = viewer;
    _root = root;
    _pointOfViewHandler = new PointOfViewHandler(_viewer);
    _pointOfViewHandler->setPlayer(this);
    
    _cannon = new Cannon(_root);

    _root->addChild(_cannon->getMatrixTransform());
}