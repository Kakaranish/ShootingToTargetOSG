#include "World.hpp"

osg::ref_ptr<osg::Geode> World::createGround(float width, float height, float thickness)
{
    const float axisRotationAngle = osg::PI;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0.0f, 0.0f, -1 * thickness), width, height, thickness);
    box->setRotation(osg::Quat(axisRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> groundDrawable(new osg::ShapeDrawable(box));
    groundDrawable->setColor(getColor(127, 255, 0));
    geode->addDrawable(groundDrawable);
    return geode;
}

osg::ref_ptr<osg::Group> World::getRoot()
{
    return _root;
}

std::vector<ShootingTarget *> *World::getShootingTargets()
{
    return &_shootingTargets;
}

osg::ref_ptr<osg::Geode> World::getGround()
{
    return _ground;
}