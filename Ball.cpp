#include "Ball.hpp"

Ball::Ball(World *world,
           osg::Vec3f startPosition,
           float radius,
           osg::Vec3f velocity) : _gravity(-0.01),
                                  _world(world),
                                  _radius(radius),
                                  _velocity(velocity)
{
    _isFalling = true;

    osg::ref_ptr<osg::ShapeDrawable> ball = new osg::ShapeDrawable(
        new osg::Sphere(startPosition, radius));
    ball->setColor(getColor(60, 60, 60));
    _ballGeode = new osg::Geode;
    _ballGeode->addDrawable(ball.get());

    _ballMatrixTransform = new osg::MatrixTransform;
    _ballMatrixTransform->addChild(_ballGeode);
    _ballMatrixTransform->setUpdateCallback(this);
    _world->getRoot()->addChild(_ballMatrixTransform);
}

void Ball::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    if (_isFalling == false)
    {
        if (_timer.time_m() > 2 * 1000)
        {
            _ballMatrixTransform->removeChild(_ballGeode);
            _ballMatrixTransform->removeUpdateCallback(this);
            _world->getRoot()->removeChild(_ballMatrixTransform);
        }
        return;
    }

    _velocity += osg::Vec3f(0, 0, _gravity);

    // Check for collision here
    // std::cout << _world->getShootingTargets()->size() << std::endl;

    if (_ballMatrixTransform->getBound().center().z() < _radius)
    {
        osg::Vec3f ballPosition = _ballMatrixTransform->getMatrix().getTrans();
        osg::Vec3f newBallPosition =
            ballPosition + osg::Vec3f(0, 0, _radius - _ballMatrixTransform->getBound().center().z());
        _ballMatrixTransform->setMatrix(osg::Matrix::translate(newBallPosition));

        _isFalling = false;
        _timer.setStartTick();
        return;
    }

    osg::Vec3f newPosition = _ballMatrixTransform->getMatrix().getTrans() + _velocity;
    _ballMatrixTransform->setMatrix(osg::Matrix::translate(newPosition));
}

osg::ref_ptr<osg::MatrixTransform> Ball::getBallMatrixTransform()
{
    return _ballMatrixTransform;
}