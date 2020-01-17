#include "Cannon.hpp"

Cannon::Cannon(osg::ref_ptr<osg::Group> world, osg::Vec3f position)
    : _defaultMovementGrowth(0.5),
      _defaultSkewAngleGrowth(1),
      _defaultBarrelSkewAngle(degreesToRadians(-60)),
      _barrelLength(4.5f),
      _wheelRadius(0.8f),
      _world(world),
      _position(position)

{
    _barrelSkewAngle = _defaultBarrelSkewAngle;
    _position += osg::Vec3f(0.f, 0.f, _wheelRadius);
    _cannonMatrixTransform = createCannonMatrixTransform(_position);
}

osg::ref_ptr<osg::MatrixTransform> Cannon::createCannonMatrixTransform(osg::Vec3f defaultPosition)
{
    const float wheelThickness = 0.45f;
    const float barrelRadius = 0.3f;

    osg::ref_ptr<osg::ShapeDrawable> leftWheelShapeDrawable = createWheelShapeDrawable(LEFT, wheelThickness);
    osg::ref_ptr<osg::ShapeDrawable> rightWheelShapeDrawable = createWheelShapeDrawable(RIGHT, wheelThickness);
    osg::ref_ptr<osg::Geode> wheelsGeode = new osg::Geode;
    wheelsGeode->addDrawable(leftWheelShapeDrawable);
    wheelsGeode->addDrawable(rightWheelShapeDrawable);

    osg::Vec3f barrelAnchorOffset = osg::Vec3f(0,
                                               (_barrelLength / 2.f) - (_barrelLength / 4.f),
                                               _wheelRadius);
    _barrelAnchor = defaultPosition + barrelAnchorOffset;
    _barrelMatrixTransform = createBarrelMatrixTransform(barrelRadius);

    osg::ref_ptr<osg::MatrixTransform> cannonMatrix = new osg::MatrixTransform;
    cannonMatrix->addChild(wheelsGeode);
    cannonMatrix->addChild(_barrelMatrixTransform);

    return cannonMatrix;
}

osg::ref_ptr<osg::ShapeDrawable> Cannon::createWheelShapeDrawable(Direction direction, float wheelThickness)
{
    float multiplier = 0;
    if (direction == LEFT)
    {
        multiplier = -1;
    }
    else if (direction == RIGHT)
    {
        multiplier = 1;
    }
    else
    {
        return NULL;
    }

    const float wheelCylinderRotationAngle = osg::PI_2;
    osg::Vec3f cylinderPosition = _position + osg::Vec3f(multiplier * wheelThickness, 0, 0);
    osg::ref_ptr<osg::Cylinder> wheelCylinder = new osg::Cylinder(cylinderPosition, _wheelRadius, wheelThickness);
    wheelCylinder->setRotation(osg::Quat(wheelCylinderRotationAngle, osg::Y_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> wheelShapeDrawable = new osg::ShapeDrawable(wheelCylinder);
    wheelShapeDrawable->setColor(getColor(205, 133, 63));

    return wheelShapeDrawable;
}

osg::ref_ptr<osg::MatrixTransform> Cannon::createBarrelMatrixTransform(float barrelRadius)
{
    osg::ref_ptr<osg::Cylinder> barrelCylinder = new osg::Cylinder(
        osg::Vec3f(0.f, 0.f, 0.f), barrelRadius, _barrelLength);
    osg::ref_ptr<osg::ShapeDrawable> barrelShapeDrawable = new osg::ShapeDrawable(barrelCylinder);
    barrelShapeDrawable->setColor(getColor(100, 100, 100));
    osg::ref_ptr<osg::Geode> barrelGeode = new osg::Geode;
    barrelGeode->addDrawable(barrelShapeDrawable);

    osg::ref_ptr<osg::MatrixTransform> barrelMatrixTransform = new osg::MatrixTransform;
    barrelMatrixTransform->setMatrix(
        osg::Matrix::rotate(osg::Quat(_barrelSkewAngle, osg::X_AXIS)) *
        osg::Matrix::translate(_barrelAnchor));
    barrelMatrixTransform->addChild(barrelGeode);

    return barrelMatrixTransform;
}

void Cannon::move(Direction direction)
{
    float movement = 0;
    if (direction == LEFT)
    {
        movement = -1 * _defaultMovementGrowth;
    }
    else if (direction == RIGHT)
    {
        movement = _defaultMovementGrowth;
    }
    else
    {
        return;
    }

    osg::Vec3f newPosition =
        _cannonMatrixTransform->getMatrix().getTrans() +
        osg::Vec3f(movement, 0.f, 0.f);

    _position += osg::Vec3f(movement, 0, 0);
    _cannonMatrixTransform->setMatrix(osg::Matrix::translate(newPosition));
}

void Cannon::skewBarrel(Direction direction)
{
    float skewAngle = 0;
    if (direction == UP)
    {
        skewAngle = _defaultSkewAngleGrowth;
    }
    else if (direction == DOWN)
    {
        skewAngle = -1 * _defaultSkewAngleGrowth;
    }
    else
    {
        return;
    }

    _barrelSkewAngle += degreesToRadians(skewAngle);
    _barrelMatrixTransform->setMatrix(
        osg::Matrix::rotate(osg::Quat(_barrelSkewAngle, osg::X_AXIS)) *
        osg::Matrix::translate(_barrelAnchor));
}

Ball *Cannon::createBall()
{
    osg::Vec3f ballOffset(
        _position.x(),
        _barrelLength / 2 * cos(osg::PI_2 - abs(_barrelSkewAngle)),
        _barrelLength / 2 * sin(osg::PI_2 - abs(_barrelSkewAngle)));
    osg::Vec3f ballPosition = _barrelAnchor + ballOffset;

    const float speed = 0.6;
    std::cout << cos(_barrelSkewAngle) * speed << ", " << sin(_barrelSkewAngle) * speed << std::endl;
    osg::Vec3f velocity(
        0,
        cos(osg::PI_2 - abs(_barrelSkewAngle)) * speed,
        sin(osg::PI_2 - abs(_barrelSkewAngle)) * speed);

    return new Ball(_world, ballPosition, 0.4, velocity);
}

osg::ref_ptr<osg::MatrixTransform> Cannon::getMatrixTransform()
{
    return _cannonMatrixTransform;
}