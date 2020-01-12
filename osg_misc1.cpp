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

class ShootingTarget
{
public:
    ShootingTarget(osg::Vec3f position, float radius, float thickness = 0.5f)
    {
        _postion = position;
        _radius = radius;
        _thickness = thickness;


        _shootingTarget = createShootingTarget(_postion, _radius, _thickness);
        _platform = NULL;
    }

    osg::ref_ptr<osg::MatrixTransform> getShootingTarget(){
        return _shootingTarget;
    }

    osg::ref_ptr<osg::Geode> getPlatform(){
        return _platform;
    }

private:
    osg::ref_ptr<osg::MatrixTransform> _shootingTarget;
    osg::ref_ptr<osg::Geode> _platform;
    osg::Vec3f _postion;
    float _radius;
    float _thickness;

    osg::ref_ptr<osg::MatrixTransform> createShootingTarget(osg::Vec3f position, float radius, float thickness = 0.05f)
    {
        const float xAxisRotationAngle = osg::PI_2;
        float stickLength = radius / 5. * 4.;
        position += osg::Vec3f(0, 0, 1 * (radius + stickLength));

        osg::ref_ptr<osg::Cylinder> blueCylinderShape = new osg::Cylinder(
            position + osg::Vec3f(0, 0, 0), radius, thickness);
        blueCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
        osg::ref_ptr<osg::ShapeDrawable> blueCylinder = new osg::ShapeDrawable(blueCylinderShape);
        blueCylinder->setColor(getColor(30, 144, 255, 255));

        osg::ref_ptr<osg::Cylinder> redCylinderShape = new osg::Cylinder(
            position + osg::Vec3f(0, -thickness, 0), radius / 3. * 2, 0.05f);
        redCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
        osg::ref_ptr<osg::ShapeDrawable> redCylinder = new osg::ShapeDrawable(redCylinderShape);
        redCylinder->setColor(getColor(255, 0, 0));

        osg::ref_ptr<osg::Cylinder> yellowCylinderShape = new osg::Cylinder(
            position + osg::Vec3f(0, -(thickness + 0.05f), 0), radius / 3., 0.05f);
        yellowCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
        osg::ref_ptr<osg::ShapeDrawable> yellowCylinder = new osg::ShapeDrawable(yellowCylinderShape);
        yellowCylinder->setColor(getColor(255, 255, 0));

        osg::ref_ptr<osg::ShapeDrawable> stickDrawable = new osg::ShapeDrawable(
            new osg::Box(osg::Vec3(position.x(), position.y(), position.z() - 1 * (radius + stickLength / 2.f)),
                         thickness * 3, thickness, stickLength));
        stickDrawable->setColor(getColor(139, 69, 19));

        osg::ref_ptr<osg::MatrixTransform> matrixTransform = new osg::MatrixTransform;
        matrixTransform->addChild(blueCylinder);
        matrixTransform->addChild(redCylinder);
        matrixTransform->addChild(yellowCylinder);
        matrixTransform->addChild(stickDrawable);
        return matrixTransform.release();
    }
};

osg::ref_ptr<osg::Geode> createCoordinateSystem(float distanceFromZero, float sphereRadius = 1.f)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    osg::ref_ptr<osg::ShapeDrawable> xPoint = new osg::ShapeDrawable(
        new osg::Sphere(osg::Vec3f(distanceFromZero, 0, 0), sphereRadius));
    xPoint->setColor(getColor(255, 0, 0));
    geode->addDrawable(xPoint);

    osg::ref_ptr<osg::ShapeDrawable> yPoint = new osg::ShapeDrawable(
        new osg::Sphere(osg::Vec3f(0, distanceFromZero, 0), sphereRadius));
    yPoint->setColor(getColor(0, 255, 0));
    geode->addDrawable(yPoint);

    osg::ref_ptr<osg::ShapeDrawable> zPoint = new osg::ShapeDrawable(
        new osg::Sphere(osg::Vec3f(0, 0, distanceFromZero), sphereRadius));
    zPoint->setColor(getColor(0, 0, 255));
    geode->addDrawable(zPoint);

    return geode.release();
}

osg::ref_ptr<osg::Geode> createGround(float width = 15.f)
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
    osg::ref_ptr<osg::Geode> coordinateSystem = createCoordinateSystem(10, 0.5f);
    osg::ref_ptr<osg::Geode> ground = createGround(15);

    ShootingTarget shootingTarget(osg::Vec3f(0, 2.f, 0), 1.5, 0.1f);
    osg::ref_ptr<osg::MatrixTransform> shootingTargetMatrix = shootingTarget.getShootingTarget();

    root->addChild(ground);
    root->addChild(shootingTargetMatrix);
    root->addChild(coordinateSystem);

    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(100, 100, 800, 600);
    viewer.setSceneData(root.get());
    return viewer.run();
}
