#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/StackedRotateAxisElement>
#include <osg/AnimationPath>
#include <osgViewer/Viewer>
#include <algorithm>
#include <osg/Geode>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include "PointOfViewHandler.hpp"
#include "Player.hpp"
#include "Cannon.hpp"
#include "Utility.hpp"
#include "Ball.hpp"
#include "ShootingTarget.hpp"
#include "World.hpp"


int main(int argc, char const *argv[])
{
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    
    World world;
    osg::ref_ptr<osg::Group> root = world.getRoot();
    
    Player *player = new Player(viewer, &world);
    viewer->addEventHandler(player->getPointOfViewHandler());

    viewer->setUpViewInWindow(100, 100, 800, 600);
    viewer->setSceneData(root.get());
    // const float camYDistance = -40;
    // viewer->getCamera()->setViewMatrix(osg::Matrix::lookAt(
    //     osg::Vec3(0, camYDistance, 2),
    //     osg::Vec3(0, 0, 0),
    //     osg::Vec3(0, -1, 1)));
    // viewer->setRunFrameScheme(osgViewer::Viewer::ON_DEMAND);
    
    // viewer->realize();
    // while(!viewer->done()) {
    //     viewer->frame(); 
    // }
    return viewer->run();
}
