#include "Cannon.hpp"

Cannon::Cannon(osg::ref_ptr<osg::Group> world, osg::Vec3f position)
    : _defaultMovement(0.5),
      _defaultSkewAngle(1),
      _wheelRadius(0.8f),
      _wheelThickness(0.45f),
      RotationAngle(osg::PI_2),
      BarrelRadius(0.3f),
      BarrelWidth(5.f),
      BarrelDefaultAngle(degreesToRadians(-60)),
      _world(world),
      _position(position)

{
    BarrelRotationAngle = BarrelDefaultAngle;
    _cannonMatrix = createCannon(_position);
}

osg::ref_ptr<osg::ShapeDrawable> Cannon::createWheelShapeDrawable(Direction direction)
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

    osg::Vec3f cylinderPosition = _position + osg::Vec3f(multiplier * _wheelThickness, 0, 0);
    osg::ref_ptr<osg::Cylinder> wheelCylinder = new osg::Cylinder(cylinderPosition, _wheelRadius, _wheelThickness);
    wheelCylinder->setRotation(osg::Quat(RotationAngle, osg::Y_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> wheelShapeDrawable = new osg::ShapeDrawable(wheelCylinder);
    wheelShapeDrawable->setColor(getColor(205, 133, 63));

    return wheelShapeDrawable;
}

osg::ref_ptr<osg::MatrixTransform> Cannon::createBarrelMatrixTransform()
{
    osg::ref_ptr<osg::Cylinder> barrelCylinder = new osg::Cylinder(
        osg::Vec3f(0.f, 0.f, 0.f), BarrelRadius, BarrelWidth);
    osg::ref_ptr<osg::ShapeDrawable> barrelShapeDrawable = new osg::ShapeDrawable(barrelCylinder);
    barrelShapeDrawable->setColor(getColor(100, 100, 100));

    osg::ref_ptr<osg::MatrixTransform> barrelMatrixTransform = new osg::MatrixTransform;
    barrelMatrixTransform->setMatrix(
        osg::Matrix::rotate(osg::Quat(BarrelRotationAngle, osg::X_AXIS)) *
        osg::Matrix::translate(barrelAnchor));
    barrelMatrixTransform->addChild(barrelShapeDrawable);

    return barrelMatrixTransform;
}

osg::ref_ptr<osg::MatrixTransform> Cannon::createCannon(osg::Vec3f defaultPosition)
{
    osg::Vec3f barrelAnchorOffset = osg::Vec3f(0, (BarrelWidth / 2.f) - (BarrelWidth / 4.f),
                                               _wheelRadius + BarrelRadius);
    barrelAnchor = defaultPosition + barrelAnchorOffset;


    osg::ref_ptr<osg::ShapeDrawable> leftWheelShapeDrawable = createWheelShapeDrawable(LEFT);
    osg::ref_ptr<osg::ShapeDrawable> rightWheelShapeDrawable = createWheelShapeDrawable(RIGHT);
    _barrelMatrixTransform = createBarrelMatrixTransform();

    osg::ref_ptr<osg::MatrixTransform> cannonMatrix = new osg::MatrixTransform;
    cannonMatrix->addChild(leftWheelShapeDrawable);
    cannonMatrix->addChild(rightWheelShapeDrawable);
    cannonMatrix->addChild(_barrelMatrixTransform);

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
    _barrelMatrixTransform->setMatrix(
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