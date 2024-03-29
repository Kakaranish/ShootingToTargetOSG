#include "World.hpp"

World::World()
{
    _root = new osg::Group;

    _ground = createGround(30, 100);
    _root->addChild(_ground);

    const float shootingTargetRadius = 1.5f;
    ShootingTarget *shootingTarget1 = new ShootingTarget(
        osg::Vec3f(0, 2.f, 0), shootingTargetRadius, 0.1f);
    _root->addChild(shootingTarget1->getShootingTargetMatrixTransform());
    _shootingTargets.push_back(shootingTarget1);

    ShootingTarget *shootingTarget2 = new ShootingTarget(
        osg::Vec3f(3.f, 7.f, 0), shootingTargetRadius, 0.1f);
    _root->addChild(shootingTarget2->getShootingTargetMatrixTransform());
    _shootingTargets.push_back(shootingTarget2);
}

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