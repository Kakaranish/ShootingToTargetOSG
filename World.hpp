#ifndef WORLD_HPP
#define WORLD_HPP

#include "ShootingTarget.hpp"

class World
{
private:
    osg::ref_ptr<osg::Group> _root;
    std::vector<ShootingTarget *> _shootingTargets;
    osg::ref_ptr<osg::Geode> _ground;

    osg::ref_ptr<osg::Geode> createGround(float width = 15.f,
                                          float height = 15.f,
                                          float thickness = 0.05f);

public:
    World()
    {
        _root = new osg::Group;

        _ground = createGround(30, 60);
        _root->addChild(_ground);

        ShootingTarget *shootingTarget1 = new ShootingTarget(
            osg::Vec3f(0, 2.f, 0), 1.5, 0.1f);
        _root->addChild(shootingTarget1->getShootingTargetMatrixTransform());
        _shootingTargets.push_back(shootingTarget1);
    }

    osg::ref_ptr<osg::Group> getRoot();
    std::vector<ShootingTarget *> *getShootingTargets();
    osg::ref_ptr<osg::Geode> getGround();
};

#endif