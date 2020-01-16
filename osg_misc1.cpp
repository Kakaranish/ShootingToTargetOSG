#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/StackedRotateAxisElement>
#include <osg/AnimationPath>
#include <osgViewer/Viewer>
#include <algorithm>
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

class ShootingTarget
{
public:
    ShootingTarget(osg::Vec3f position, float radius, float thickness = 0.5f)
    {
        _postion = position;
        _radius = radius;
        _thickness = thickness;

        _shootingTarget = createShootingTarget();
        _platform = createShootingTargetPlatform();

        _apcb = new osg::AnimationPathCallback;
        _apcb->setAnimationPath(createAnimationPath(5));

        _shootingTarget->setUpdateCallback(_apcb.get());
    }

    osg::ref_ptr<osg::Group> getShootingTarget()
    {
        return _shootingTarget;
    }

    osg::ref_ptr<osg::Geode> getPlatform()
    {
        return _platform;
    }

    float getPlatformWidth()
    {
        return 4. * _radius;
    }

    float getPlatformHeight()
    {
        return 1. / 3. * _radius;
    }

    float getThickness()
    {
        return _thickness;
    }

private:
    osg::ref_ptr<osg::MatrixTransform> _shootingTarget;
    osg::ref_ptr<osg::Geode> _platform;
    osg::ref_ptr<osg::AnimationPathCallback> _apcb;
    osg::Vec3f _postion;
    float _radius;
    float _thickness;

    osg::ref_ptr<osg::MatrixTransform> createShootingTarget()
    {
        const float xAxisRotationAngle = osg::PI_2;
        float stickLength = _radius / 5. * 4.;
        osg::Vec3f position(_postion);
        position += osg::Vec3f(0, 0, 1 * (_radius + stickLength + (getPlatformHeight() / 2.f)));

        osg::ref_ptr<osg::Cylinder> blueCylinderShape = new osg::Cylinder(
            position + osg::Vec3f(0, 0, 0), _radius, _thickness);
        blueCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
        osg::ref_ptr<osg::ShapeDrawable> blueCylinder = new osg::ShapeDrawable(blueCylinderShape);
        blueCylinder->setColor(getColor(30, 144, 255, 255));

        osg::ref_ptr<osg::Cylinder> redCylinderShape = new osg::Cylinder(
            position + osg::Vec3f(0, -_thickness, 0), _radius / 3. * 2, 0.05f);
        redCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
        osg::ref_ptr<osg::ShapeDrawable> redCylinder = new osg::ShapeDrawable(redCylinderShape);
        redCylinder->setColor(getColor(255, 0, 0));

        osg::ref_ptr<osg::Cylinder> yellowCylinderShape = new osg::Cylinder(
            position + osg::Vec3f(0, -(_thickness + 0.05f), 0), _radius / 3., 0.05f);
        yellowCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
        osg::ref_ptr<osg::ShapeDrawable> yellowCylinder = new osg::ShapeDrawable(yellowCylinderShape);
        yellowCylinder->setColor(getColor(255, 255, 0));

        osg::ref_ptr<osg::ShapeDrawable> stickDrawable = new osg::ShapeDrawable(
            new osg::Box(osg::Vec3(position.x(), position.y(), position.z() - 1 * (_radius + stickLength / 2.f)),
                         _thickness * 3, _thickness, stickLength));
        stickDrawable->setColor(getColor(139, 69, 19));

        osg::ref_ptr<osg::MatrixTransform> matrixTransform = new osg::MatrixTransform;
        matrixTransform->addChild(blueCylinder);
        matrixTransform->addChild(redCylinder);
        matrixTransform->addChild(yellowCylinder);
        matrixTransform->addChild(stickDrawable);
        return matrixTransform.release();
    }

    osg::ref_ptr<osg::Geode> createShootingTargetPlatform()
    {
        osg::Vec3f position(_postion);
        position -= osg::Vec3f(0, 0, -1 * (getPlatformHeight() / 2.));
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        osg::ref_ptr<osg::Box> box = new osg::Box(position, getPlatformWidth(),
                                                  getPlatformHeight(), getPlatformHeight());
        osg::ref_ptr<osg::ShapeDrawable> groundDrawable(new osg::ShapeDrawable(box));
        groundDrawable->setColor(getColor(169, 169, 169));
        geode->addDrawable(groundDrawable);
        return geode;
    }

    osg::ref_ptr<osg::AnimationPath> createAnimationPath(float time)
    {
        osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
        path->setLoopMode(osg::AnimationPath::LOOP);

        float movement = getPlatformWidth() / 2.f - getThickness();
        float totalMovement = movement * 4;
        unsigned numSamples = 100;
        float deltaTime = time / (float)numSamples;
        float deltaMovement = totalMovement / (float)numSamples;

        for (size_t i = 0; i < numSamples / 4; i++)
        {
            osg::AnimationPath::ControlPoint pos(osg::Vec3f(i * deltaMovement, 0.f, 0.f));
            path->insert(deltaTime * (float)i, pos);
        }

        float timeOffset = numSamples / 4 * deltaTime;
        for (size_t i = 0; i < numSamples / 2; i++)
        {
            osg::AnimationPath::ControlPoint pos(
                osg::Vec3f(movement - (i * deltaMovement), 0.f, 0.f));
            path->insert(timeOffset + deltaTime * (float)i, pos);
        }

        timeOffset += numSamples / 2 * deltaTime;
        for (size_t i = 0; i <= numSamples / 4; i++)
        {
            osg::AnimationPath::ControlPoint pos(
                osg::Vec3f(-1 * movement + (i * deltaMovement), 0.f, 0.f));
            path->insert(timeOffset + deltaTime * (float)i, pos);
        }

        return path.release();
    }
};

osg::ref_ptr<osg::Geode> createGround(float width = 15.f, float height = 15.f)
{
    const float axisRotationAngle = osg::PI;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), width, width, 0.05f);
    box->setRotation(osg::Quat(axisRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> groundDrawable(new osg::ShapeDrawable(box));
    groundDrawable->setColor(getColor(127, 255, 0));
    geode->addDrawable(groundDrawable);
    return geode;
}

int main(int argc, char const *argv[])
{
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osg::Geode> ground = createGround(30);

    ShootingTarget shootingTarget(osg::Vec3f(0, 2.f, 0), 1.5, 0.1f);
    osg::ref_ptr<osg::Group> shootingTargetMatrix = shootingTarget.getShootingTarget();
    osg::ref_ptr<osg::Geode> shootingTargetPlatformGeode = shootingTarget.getPlatform();

    root->addChild(ground);
    root->addChild(shootingTargetMatrix);
    root->addChild(shootingTargetPlatformGeode);
    
    
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    Player *player = new Player(viewer, root);
    viewer->addEventHandler(player->getPointOfViewHandler());

    viewer->setUpViewInWindow(100, 100, 800, 600);
    const float camYDistance = -40;
    viewer->getCamera()->setViewMatrix(osg::Matrix::lookAt(
        osg::Vec3(0, -camYDistance, 2),
        osg::Vec3(0, 0, 0),
        osg::Vec3(0, 0, 1)));
    viewer->setRunFrameScheme(osgViewer::Viewer::ON_DEMAND);
    viewer->setSceneData(root.get());

    return viewer->run();
}
