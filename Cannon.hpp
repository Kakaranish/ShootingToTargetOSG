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
    osg::ref_ptr<osg::MatrixTransform> _cannonMatrixTransform;
    std::vector<Ball *> balls;
    osg::ref_ptr<osg::MatrixTransform> createCannonMatrixTransform(osg::Vec3f defaultPosition = osg::Vec3f(0, -20, 0));
    const float _defaultMovementGrowth;
    const float _defaultSkewAngleGrowth;
    const float _defaultBarrelSkewAngle;
    const float _barrelLength;
    const float _wheelRadius;

    float _barrelSkewAngle;

    osg::ref_ptr<osg::MatrixTransform> _barrelMatrixTransform;
    osg::ref_ptr<osg::Group> _world;
    osg::Vec3f _position;
    osg::Vec3f _barrelAnchor;

    osg::ref_ptr<osg::ShapeDrawable> createWheelShapeDrawable(Direction direction, float wheelThicknes);
    osg::ref_ptr<osg::MatrixTransform> createBarrelMatrixTransform(float radius);

public:
    Cannon(osg::ref_ptr<osg::Group> world, osg::Vec3f position = osg::Vec3f(0, -20, 0));

    void move(Direction direction);
    void skewBarrel(Direction direction);
    
    Ball *createBall();

    osg::ref_ptr<osg::MatrixTransform> getMatrixTransform();
};

#endif