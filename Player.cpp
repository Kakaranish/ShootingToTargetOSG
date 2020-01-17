#include "Player.hpp"
#include "PointOfViewHandler.hpp"

Player::Player(osg::ref_ptr<osgViewer::Viewer> viewer, World *world)
    : _world(world)
{
    _root = world->getRoot();
    _pointOfViewHandler = new PointOfViewHandler(viewer);
    _pointOfViewHandler->setPlayer(this);

    _cannon = new Cannon(_world);
    _root->addChild(_cannon->getMatrixTransform());
}

void Player::shoot()
{
    _cannon->createBall();
}

osg::ref_ptr<osg::Group> Player::getRoot()
{
    return _root;
}

PointOfViewHandler *Player::getPointOfViewHandler()
{
    return _pointOfViewHandler;
}

Cannon *Player::getCannon()
{
    return _cannon;
}
