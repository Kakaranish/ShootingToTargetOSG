#include "Cannon.hpp"

osg::ref_ptr<osg::MatrixTransform> Cannon::createCannon(osg::Vec3f defaultPosition)
{
    osg::ref_ptr<osg::MatrixTransform> cannonMatrix = new osg::MatrixTransform;

    osg::ref_ptr<osg::Cylinder> leftWheelCylinder = new osg::Cylinder(
        defaultPosition + osg::Vec3f(-WheelThickness, 0, 0), WheelRadius, WheelThickness);
    leftWheelCylinder->setRotation(osg::Quat(RotationAngle, osg::Y_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> leftWheel = new osg::ShapeDrawable(leftWheelCylinder);
    leftWheel->setColor(getColor(205, 133, 63));

    osg::ref_ptr<osg::Cylinder> rightWheelCylinder = new osg::Cylinder(
        defaultPosition + osg::Vec3f(WheelThickness, 0, 0), WheelRadius, WheelThickness);
    rightWheelCylinder->setRotation(osg::Quat(RotationAngle, osg::Y_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> rightWheel = new osg::ShapeDrawable(rightWheelCylinder);
    rightWheel->setColor(getColor(205, 133, 63));

    osg::ref_ptr<osg::Cylinder> barrelCylinder = new osg::Cylinder(
        defaultPosition + osg::Vec3f(0, BarrelWidth / 2.f - BarrelWidth / 4.f, WheelRadius + BarrelRadius),
        BarrelRadius, BarrelWidth);
    barrelCylinder->setRotation(osg::Quat(BarrelRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> barrel = new osg::ShapeDrawable(barrelCylinder);
    barrel->setColor(getColor(100, 100, 100));

    cannonMatrix->addChild(leftWheel);
    cannonMatrix->addChild(rightWheel);
    cannonMatrix->addChild(barrel);

    return cannonMatrix;
}

osg::ref_ptr<osg::MatrixTransform> Cannon::getMatrixTransform()
{
    return _cannonMatrix;
}