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
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include "PointOfViewHandler.hpp"
#include "Player.hpp"

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

class SomeCallback : public osg::NodeCallback
{
public:
    osg::ref_ptr<osg::MatrixTransform> ballMatrix;
    osg::ref_ptr<osg::Geode> toCollide;
    osg::ref_ptr<osg::Sphere> sphere;
    osg::ref_ptr<osg::Geode> geode;
    float speed;
    float radius;
    bool finished;
    float xSpeed;

    SomeCallback(osg::ref_ptr<osg::Geode> _toCollide)
    {
        xSpeed = 0.03;
        finished = false;
        speed = 0.4;
        radius = 0.4;
        toCollide = _toCollide;
        sphere = new osg::Sphere(osg::Vec3f(0, 0, 6), radius);
        osg::ref_ptr<osg::ShapeDrawable> ball = new osg::ShapeDrawable(sphere);
        ball->setColor(getColor(112, 255, 0));
        geode = new osg::Geode;
        geode->addDrawable(ball.get());

        ballMatrix = new osg::MatrixTransform;
        ballMatrix->setUpdateCallback(this);
        ballMatrix->addChild(geode.get());
    }
    void operator()(osg::Node *node, osg::NodeVisitor *nv)
    {
        osg::ref_ptr<osg::Geode> x;
        float acceleration = -0.005;
        // std::cout << "Hello " << std::endl;
        // osg::Node* x = ballMatrix->asNode();
        // std::cout << x << std::endl;

        speed += acceleration;
        if (speed < -0.3)
        {
            speed = -0.3;
        }

        // std::cout << dynamic_cast<osg::Sphere*>(geode->getChild(0)->asDrawable()) << std::endl;
        // dynamic_cast<osg::Sphere>
        // geode->getChild(0)->asDrawable()->getBoundingBox()
        // std::cout << geode->getChild(0)->asDrawable()->getBoundingBox()<< std::endl;
        // std::cout << "1st: " << ballMatrix->getBound().center().z() << std::endl;
        std::cout << ballMatrix->getBound().center().x() << ", " << ballMatrix->getBound().center().y() << ", " << ballMatrix->getBound().center().z() << std::endl;
        std::cout << ballMatrix->getBound().radius() << std::endl;

        if (ballMatrix->getBound().center().x() >= 3)
        {
            xSpeed = 0;
        }
        if (finished)
        {
            return;
        }
        if (ballMatrix->getBound().center().z() == radius)
        {
            return;
        }

        if (ballMatrix->getBound().center().z() < radius)
        {
            osg::Vec3f pos = ballMatrix->getMatrix().getTrans();
            float diff = -ballMatrix->getBound().center().z();
            pos += osg::Vec3f(0, 0, diff + radius);

            ballMatrix->setMatrix(osg::Matrix::translate(pos));
            std::cout << "2nd: " << ballMatrix->getBound().center().z() << std::endl;
            finished = true;
            return;
        }

        // // std::cout << pos.z() << std::endl;
        // // if(pos.z() == radius){
        // //     return;
        // // }
        osg::Vec3f pos = ballMatrix->getMatrix().getTrans();
        osg::Vec3f newPos = pos + osg::Vec3f(xSpeed, 0, speed);
        ballMatrix->setMatrix(osg::Matrix::translate(newPos));

        // }
        // ballMatrix->;

        // osg::ref_ptr<osg::Geode> geode = node->asGeode();
        // osg::ref_ptr<osg::MatrixTransform> matrix = new osg::MatrixTransform();
        // matrix->addChild(matrix);
        // std::cout << "Hello world" << std::endl;
        // matrix->setMatrix(osg::Matrix::translate(osg::Vec3f(2.0, 2.0, 0)));
        // std::cout << matrix << std::endl;
    }
};

// class PointOfViewHandler : public osgGA::GUIEventHandler
// {
//     osg::observer_ptr<osgViewer::Viewer> _viewer;
//     bool allowEventFocus;

// public:
//     bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//     {
//         if (ea.getEventType() != osgGA::GUIEventAdapter::KEYDOWN)
//         {
//             return false;
//         }

//         switch (ea.getUnmodifiedKey())
//         {
//         case osgGA::GUIEventAdapter::KEY_Up:
//             std::cout << "Key up " << allowEventFocus << std::endl;
//             break;
//         case osgGA::GUIEventAdapter::KEY_Right:
//             std::cout << "Key right" << std::endl;
//             break;
//         case osgGA::GUIEventAdapter::KEY_Down:
//             std::cout << "Key down" << std::endl;
//             break;
//         case osgGA::GUIEventAdapter::KEY_Left:
//             std::cout << "Key left" << std::endl;
//             break;
//         case osgGA::GUIEventAdapter::KEY_Space:
//             std::cout << "Space" << std::endl;
//             break;
//         default:
//             return false;
//         }

//         return true;
//     }

//     PointOfViewHandler(const osg::ref_ptr<osgViewer::Viewer> &viewer)
//     {
//         _viewer = viewer;
//         allowEventFocus = true;

//         _viewer->getCamera()->setAllowEventFocus(allowEventFocus);
//     }

// protected:
//     virtual ~PointOfViewHandler()
//     {
//     }
// };

// class Player
// {
//     osg::ref_ptr<osgViewer::Viewer> _viewer;
//     PointOfViewHandler* _pointOfViewHandler;

// public:
//     Player(osg::ref_ptr<osgViewer::Viewer> viewer)
//     {
//         _viewer = viewer;
//         _pointOfViewHandler = new PointOfViewHandler(_viewer);
        
//     }
// };


osg::ref_ptr<osg::MatrixTransform> createCannon(osg::Vec3f defaultPosition = osg::Vec3f(0, -20, 0))
{
    osg::ref_ptr<osg::MatrixTransform> cannonMatrix = new osg::MatrixTransform;

    const float wheelRadius = 0.8f;
    const float wheelThickness = 0.45f;
    const float rotationAngle = osg::PI_2;
    const float barrelRadius = 0.3f;
    const float barrelWidth = 5.f;
    const float barrelRotationAngle = -0.8 * osg::PI_2;

    osg::ref_ptr<osg::Cylinder> leftWheelCylinder = new osg::Cylinder(
        defaultPosition + osg::Vec3f(-wheelThickness, 0, 0), wheelRadius, wheelThickness);
    leftWheelCylinder->setRotation(osg::Quat(rotationAngle, osg::Y_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> leftWheel = new osg::ShapeDrawable(leftWheelCylinder);
    leftWheel->setColor(getColor(205, 133, 63));

    osg::ref_ptr<osg::Cylinder> rightWheelCylinder = new osg::Cylinder(
        defaultPosition + osg::Vec3f(wheelThickness, 0, 0), wheelRadius, wheelThickness);
    rightWheelCylinder->setRotation(osg::Quat(rotationAngle, osg::Y_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> rightWheel = new osg::ShapeDrawable(rightWheelCylinder);
    rightWheel->setColor(getColor(205, 133, 63));

    osg::ref_ptr<osg::Cylinder> barrelCylinder = new osg::Cylinder(
        defaultPosition + osg::Vec3f(0, barrelWidth / 2.f - barrelWidth / 4.f, wheelRadius + barrelRadius),
        barrelRadius, barrelWidth);
    barrelCylinder->setRotation(osg::Quat(barrelRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> barrel = new osg::ShapeDrawable(barrelCylinder);
    barrel->setColor(getColor(100, 100, 100));

    cannonMatrix->addChild(leftWheel);
    cannonMatrix->addChild(rightWheel);
    cannonMatrix->addChild(barrel);

    return cannonMatrix;
}

int main(int argc, char const *argv[])
{
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osg::Geode> coordinateSystem = createCoordinateSystem(10, 0.5f);
    osg::ref_ptr<osg::Geode> ground = createGround(30);

    ShootingTarget shootingTarget(osg::Vec3f(0, 2.f, 0), 1.5, 0.1f);
    osg::ref_ptr<osg::Group> shootingTargetMatrix = shootingTarget.getShootingTarget();
    osg::ref_ptr<osg::Geode> shootingTargetPlatformGeode = shootingTarget.getPlatform();

    // osg::ref_ptr<osg::ShapeDrawable> yPoint = new osg::ShapeDrawable(
    //     new osg::Sphere(osg::Vec3f(20, 0, 0), 1));
    // yPoint->setColor(getColor(0, 255, 0));
    // osg::ref_ptr<osg::MatrixTransform> matrix = new osg::MatrixTransform;
    // matrix->addChild(yPoint);

    float camYDistance = -40;

    // osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3f(0, camYDistance + 10, 0), 2);
    // osg::ref_ptr<osg::ShapeDrawable> ball1 = new osg::ShapeDrawable(sphere);
    // ball1->setColor(getColor(112, 255, 0));
    // osg::ref_ptr<osg::MatrixTransform> ballMatrix1 = new osg::MatrixTransform;
    // ballMatrix1->addChild(ball1);
    // // root->addChild(ballMatrix1);

    osg::ref_ptr<osg::MatrixTransform> cannonMatrix = createCannon();

    // SomeCallback ball(ground);
    root->addChild(ground);
    root->addChild(shootingTargetMatrix);
    root->addChild(shootingTargetPlatformGeode);
    root->addChild(coordinateSystem);
    // root->addChild(ball.ballMatrix.get());
    root->addChild(cannonMatrix);

    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    Player player(viewer);
    viewer->addEventHandler(player.getPointOfViewHandler());

    viewer->setUpViewInWindow(100, 100, 800, 600);
    viewer->getCamera()->setViewMatrix(osg::Matrix::lookAt(
        osg::Vec3(0, -camYDistance, 2),
        osg::Vec3(0, 0, 0),
        osg::Vec3(0, 0, 1)));
    viewer->setRunFrameScheme(osgViewer::Viewer::ON_DEMAND);
    viewer->setSceneData(root.get());

    return viewer->run();
}
