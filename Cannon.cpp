#include "Cannon.hpp"

Cannon::Cannon(osg::ref_ptr<osg::Group> world, osg::Vec3f position)
    : _defaultMovement(0.5),
      _defaultSkewAngle(1),
      _wheelRadius(0.8f),
      _wheelThickness(0.45f),
      RotationAngle(osg::PI_2),
      BarrelRadius(0.3f),
      BarrelWidth(5.f),
      BarrelDefaultAngle(degreesToRadians(-60))

{
    _position = position;
    BarrelRotationAngle = BarrelDefaultAngle;
    _world = world;
    _cannonMatrix = createCannon(_position);
}

osg::ref_ptr<osg::MatrixTransform> Cannon::createCannon(osg::Vec3f defaultPosition)
{
    osg::ref_ptr<osg::MatrixTransform> cannonMatrix = new osg::MatrixTransform;

    osg::ref_ptr<osg::Cylinder> leftWheelCylinder = new osg::Cylinder(
        defaultPosition + osg::Vec3f(-_wheelThickness, 0, 0), _wheelRadius, _wheelThickness);
    leftWheelCylinder->setRotation(osg::Quat(RotationAngle, osg::Y_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> leftWheel = new osg::ShapeDrawable(leftWheelCylinder);
    leftWheel->setColor(getColor(205, 133, 63));

    osg::ref_ptr<osg::Cylinder> rightWheelCylinder = new osg::Cylinder(
        defaultPosition + osg::Vec3f(_wheelThickness, 0, 0), _wheelRadius, _wheelThickness);
    rightWheelCylinder->setRotation(osg::Quat(RotationAngle, osg::Y_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> rightWheel = new osg::ShapeDrawable(rightWheelCylinder);
    rightWheel->setColor(getColor(205, 133, 63));

    // BarrelWidth * sin(BarrelRotationAngle) / 2.f) - (sin(BarrelRotationAngle) * BarrelWidth / 4.f),

    barrelAnchor =
        defaultPosition +
        osg::Vec3f(0,
                   (BarrelWidth / 2.f) - (BarrelWidth / 4.f),
                   _wheelRadius + BarrelRadius);
    barrelCylinder = new osg::Cylinder(osg::Vec3f(), BarrelRadius, BarrelWidth);
    // barrelCylinder->setRotation(osg::Quat(BarrelRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> barrel = new osg::ShapeDrawable(barrelCylinder);
    barrel->setColor(getColor(100, 100, 100));

    barrelGeode = new osg::Geode;
    barrelGeode->addChild(barrel);

    barrelMatrix = new osg::MatrixTransform;
    // std::cout << BarrelRotationAngle << std::endl;
    barrelMatrix->setMatrix(
        osg::Matrix::rotate(osg::Quat(BarrelRotationAngle, osg::X_AXIS)) *
        osg::Matrix::translate(barrelAnchor));
    barrelMatrix->addChild(barrel);

    cannonMatrix->addChild(leftWheel);
    cannonMatrix->addChild(rightWheel);
    cannonMatrix->addChild(barrelMatrix);
    // _world->addChild(barrelMatrix);

    return cannonMatrix;
}

void Cannon::move(Direction direction)
{
    float movement = 0;
    if (direction == LEFT)
    {
        movement = -1 * _defaultMovement;
    }
    else if (direction == RIGHT)
    {
        movement = _defaultMovement;
    }
    else
    {
        return;
    }

    osg::Vec3f newPosition =
        _cannonMatrix->getMatrix().getTrans() +
        osg::Vec3f(movement, 0.f, 0.f);

    _position += osg::Vec3f(movement, 0, 0);
    _cannonMatrix->setMatrix(osg::Matrix::translate(newPosition));
}

void Cannon::skewBarrel(Direction direction)
{
    float skewAngle = 0;
    if (direction == UP)
    {
        skewAngle = _defaultSkewAngle;
    }
    else if (direction == DOWN)
    {
        skewAngle = -1 * _defaultSkewAngle;
    }
    else
    {
        return;
    }

    BarrelRotationAngle += degreesToRadians(skewAngle);
    barrelMatrix->setMatrix(
        osg::Matrix::rotate(osg::Quat(BarrelRotationAngle, osg::X_AXIS)) *
        osg::Matrix::translate(barrelAnchor));
}

Ball *Cannon::createBall()
{
    osg::Vec3f ballOffset(
        _position.x(),
        BarrelWidth / 2 * cos(osg::PI_2 - abs(BarrelRotationAngle)),
        BarrelWidth / 2 * sin(osg::PI_2 - abs(BarrelRotationAngle)));
    osg::Vec3f ballPosition = barrelAnchor + ballOffset;

    const float speed = 0.6;
    std::cout << cos(BarrelRotationAngle) * speed << ", " << sin(BarrelRotationAngle) * speed << std::endl;
    osg::Vec3f velocity(
        0,
        cos(osg::PI_2 - abs(BarrelRotationAngle)) * speed,
        sin(osg::PI_2 - abs(BarrelRotationAngle)) * speed);

    return new Ball(_world, ballPosition, 0.4, velocity);
}

osg::ref_ptr<osg::MatrixTransform> Cannon::getMatrixTransform()
{
    return _cannonMatrix;
}