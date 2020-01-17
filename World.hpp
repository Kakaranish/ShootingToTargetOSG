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
    World();

    osg::ref_ptr<osg::Group> getRoot();
    std::vector<ShootingTarget *> *getShootingTargets();
    osg::ref_ptr<osg::Geode> getGround();
};

#endif