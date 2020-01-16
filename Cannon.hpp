#ifndef CANNON_HPP
#define CANNON_HPP

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include "Utility.hpp"
#include "Ball.hpp"
#include <math.h>

class Cannon
{
private:
    osg::ref_ptr<osg::MatrixTransform> _cannonMatrix;
    std::vector<Ball *> balls;
    osg::ref_ptr<osg::MatrixTransform> createCannon(osg::Vec3f defaultPosition = osg::Vec3f(0, -20, 0));
    const float _defaultMovement;
    const float _defaultSkewAngle;
    const float _wheelRadius;
    const float _wheelThickness;
    const float RotationAngle;
    const float BarrelRadius;
    const float BarrelWidth;
    const float BarrelDefaultAngle;
    float BarrelRotationAngle;

    osg::Vec3f barrelAnchor;
    osg::ref_ptr<osg::MatrixTransform> _barrelMatrixTransform;
    osg::ref_ptr<osg::Group> _world;
    osg::Vec3f _position;

    osg::ref_ptr<osg::ShapeDrawable> createWheelShapeDrawable(Direction direction);
    osg::ref_ptr<osg::MatrixTransform> createBarrelMatrixTransform();

public:
    Cannon(osg::ref_ptr<osg::Group> world, osg::Vec3f position = osg::Vec3f(0, -20, 0));

    void move(Direction direction);
    void skewBarrel(Direction direction);
    Ball *createBall();
    osg::ref_ptr<osg::MatrixTransform> getMatrixTransform();
};

#endif