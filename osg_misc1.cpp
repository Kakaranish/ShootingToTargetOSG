#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/StackedRotateAxisElement>
#include <osgViewer/Viewer>
#include <osg/AnimationPath>
#include <algorithm>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>

osg::Vec4f getColor(int r, int g, int b, int alpha = 255)
{
    return osg::Vec4f(r / 255., g / 255., b / 255., alpha / 255.);
}

osg::ref_ptr<osg::MatrixTransform> createShootingTarget(float radius, float thickness = 0.05f)
{
    const float xAxisRotationAngle = -90;
    osg::ref_ptr<osg::Cylinder> blueCylinderShape = new osg::Cylinder(
        osg::Vec3f(0, 0, 0), radius, thickness);
    blueCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> blueCylinder = new osg::ShapeDrawable(blueCylinderShape);
    blueCylinder->setColor(getColor(30, 144, 255, 255));

    osg::ref_ptr<osg::Cylinder> redCylinderShape = new osg::Cylinder(
        osg::Vec3f(0, 0, thickness), radius / 3. * 2, 0.05f);
    redCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> redCylinder = new osg::ShapeDrawable(redCylinderShape);
    redCylinder->setColor(getColor(255, 0, 0, 255));

    osg::ref_ptr<osg::Cylinder> yellowCylinderShape = new osg::Cylinder(
        osg::Vec3f(0, 0, thickness + 0.05f), radius / 3., 0.05f);
    yellowCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> yellowCylinder = new osg::ShapeDrawable(yellowCylinderShape);
    yellowCylinder->setColor(getColor(255, 255, 0, 255));

    osg::ref_ptr<osg::MatrixTransform> matrixTransform = new osg::MatrixTransform;
    matrixTransform->addChild(blueCylinder);
    matrixTransform->addChild(redCylinder);
    matrixTransform->addChild(yellowCylinder);
    return matrixTransform.release();
}

int main(int argc, char const *argv[])
{
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osg::MatrixTransform> shootingTarget = createShootingTarget(3, 0.1f);

    root->addChild(shootingTarget);

    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(100, 100, 800, 600);
    viewer.setSceneData(root.get());
    return viewer.run();
}
