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
        new osg::Sphere(osg::Vec3f(), radius));
    ball->setColor(getColor(60, 60, 60));
    _ballGeode = new osg::Geode;
    _ballGeode->addDrawable(ball.get());

    _ballMatrixTransform = new osg::MatrixTransform;
    _ballMatrixTransform->setMatrix(osg::Matrix::translate(startPosition));
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

    if (_ballMatrixTransform->getBound().center().z() < _radius)
    {
        osg::Vec3f currentPosition = getCurrentPosition();
        osg::Vec3 nextPosition(currentPosition.x(), currentPosition.y(), _radius);
        _ballMatrixTransform->setMatrix(osg::Matrix::translate(nextPosition));

        _isFalling = false;
        _timer.setStartTick();
        return;
    }

    _velocity += osg::Vec3f(0, 0, _gravity);

    ShootingTarget *collidingShootingTarget = getCollidingShootingTarget();
    if (collidingShootingTarget != nullptr)
    {
        bool isFalling = _velocity.z() < 0;
        if (isFalling)
        {
            _velocity = osg::Vec3f(_velocity.x(), 0, _velocity.z());
        }
        else
        {
            _velocity = osg::Vec3f(_velocity.x(), 0, 0);
        }
    }

    osg::Vec3f newPosition = calculateNextPosition();
    _ballMatrixTransform->setMatrix(osg::Matrix::translate(newPosition));
}

osg::ref_ptr<osg::MatrixTransform> Ball::getBallMatrixTransform()
{
    return _ballMatrixTransform;
}

osg::Vec3f Ball::getCurrentPosition()
{
    return _ballMatrixTransform->getMatrix().getTrans();
}

osg::Vec3f Ball::calculateNextPosition()
{
    return getCurrentPosition() + _velocity;
}

ShootingTarget *Ball::getCollidingShootingTarget()
{
    for (auto &&shootingTarget : *_world->getShootingTargets())
    {
        if (collidesWithShootingTarget(shootingTarget))
        {
            return shootingTarget;
        }
    }

    return nullptr;
}

bool Ball::collidesWithShootingTarget(ShootingTarget *shootingTarget)
{
    osg::Vec3f currentPosition = _ballMatrixTransform->getMatrix().getTrans();
    osg::Vec3f nextPosition = currentPosition + _velocity;
    osg::Vec3f targetCenter = shootingTarget->getTargetCenter();

    float targetYMin = targetCenter.y() - shootingTarget->getTotalTargetThickness() / 2.f;
    float targetYMax = targetCenter.y() + shootingTarget->getTotalTargetThickness() / 2.f;

    if ((currentPosition.y() >= targetYMin && currentPosition.y() <= targetYMax) ||
        (nextPosition.y() >= targetYMin && nextPosition.y() <= targetYMax) ||
        (currentPosition.y() < targetYMin && nextPosition.y() > targetYMax))
    {
        osg::Vec3f midPosition = (currentPosition + nextPosition) / 2.f;
        float diffXSquare = pow(midPosition.x() - targetCenter.x(), 2);
        float diffZSquare = pow(midPosition.z() - targetCenter.z(), 2);
        return sqrt(diffXSquare + diffZSquare) < shootingTarget->getTargetRadius();
    }
    else
    {
        return false;
    }
}