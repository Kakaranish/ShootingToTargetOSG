#ifndef POINTOFVIEWHANDLER_HPP
#define POINTOFVIEWHANDLER_HPP

#include <iostream>
#include <osgViewer/Viewer>
#include <osg/AnimationPath>

class Player;

class PointOfViewHandler : public osgGA::GUIEventHandler
{
private:
    osg::observer_ptr<osgViewer::Viewer> _viewer;
    bool allowEventFocus;
public:
    Player* _player;
    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    PointOfViewHandler(const osg::ref_ptr<osgViewer::Viewer> &viewer);
    void setPlayer(Player* player);
    osg::ref_ptr<osg::AnimationPath> createAnimationPath(float time, float movement);
};

#endif