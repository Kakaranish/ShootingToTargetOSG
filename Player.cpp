#include "Player.hpp"
#include "PointOfViewHandler.hpp"

Player::Player(osg::ref_ptr<osgViewer::Viewer> viewer, osg::ref_ptr<osg::Group> root)
{
    _root = root;
    _pointOfViewHandler = new PointOfViewHandler(viewer);
    _pointOfViewHandler->setPlayer(this);

    _cannon = new Cannon(_root);
    _root->addChild(_cannon->getMatrixTransform());
}

void Player::shoot()
{
    Ball *ball = _cannon->createBall();
    _root->addChild(ball->getBallMatrixTransform());
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
