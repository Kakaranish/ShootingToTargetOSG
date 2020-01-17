#ifndef SHOOTINGTARGET_HPP
#define SHOOTINGTARGET_HPP

#include "Utility.hpp"
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/AnimationPath>

class ShootingTarget
{
private:
    osg::ref_ptr<osg::MatrixTransform> _shootingTargetMatrixTransform;
    osg::ref_ptr<osg::Geode> _targetGeode;
    osg::ref_ptr<osg::Geode> _stickGeode;
    osg::ref_ptr<osg::Geode> _platformGeode;
    osg::ref_ptr<osg::MatrixTransform> _movingElementsMatrixTransform;
    osg::ref_ptr<osg::AnimationPathCallback> _apcb;
    osg::Vec3f _postion;
    float _radius;
    float _thickness;

    float getPlatformWidth();
    float getPlatformHeight();
    float getThickness();
    float getStickLength();

    osg::ref_ptr<osg::Geode> createTargetGeode();
    osg::ref_ptr<osg::Geode> createStickGeode();
    osg::ref_ptr<osg::Geode> createPlatformGeode();
    osg::ref_ptr<osg::MatrixTransform> createMovingElementsMatrixTransform();
    osg::ref_ptr<osg::MatrixTransform> createShootingTargetMatrix();
    osg::ref_ptr<osg::AnimationPath> createAnimationPath(float time);

public:
    ShootingTarget(osg::Vec3f position, float radius, float thickness = 0.5f);
    osg::ref_ptr<osg::MatrixTransform> getShootingTargetMatrixTransform();

    osg::ref_ptr<osg::Geode> getTargetGeode();
    osg::ref_ptr<osg::Geode> getPlatformGeode();
    osg::ref_ptr<osg::Geode> getStickGeode();
};

#endif