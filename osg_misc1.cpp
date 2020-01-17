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

osg::ref_ptr<osg::Geode> createGround(float width = 15.f, float height = 15.f, float thickness = 0.05f)
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

int main(int argc, char const *argv[])
{
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    osg::ref_ptr<osg::Group> root = new osg::Group;
    
    osg::ref_ptr<osg::Geode> ground = createGround(30, 60);
    root->addChild(ground);

    ShootingTarget shootingTarget(osg::Vec3f(0, 2.f, 0), 1.5, 0.1f);
    root->addChild(shootingTarget.getShootingTargetMatrixTransform());
    
    Player *player = new Player(viewer, root);
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
