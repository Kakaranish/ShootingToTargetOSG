#include "Utility.hpp"

osg::Vec4f getColor(int r, int g, int b, int alpha)
{
    return osg::Vec4f(r / 255., g / 255., b / 255., alpha / 255.);
}

double degreesToRadians(float degrees)
{
    return degrees * M_PI / 180.f;
}

int randomInt(int start, int end)
{
    std::random_device randomDevice;
    std::mt19937 engine(randomDevice());
    std::uniform_int_distribution<> distr(start, end);
    int randomNumber = distr(engine);

    return randomNumber;
}

osg::ref_ptr<osg::Geode> createCoordinateSystem(float distanceFromZero, float sphereRadius)
{
    osg::ref_ptr<osg::Geode> coordinateSystemGeode = new osg::Geode;

    osg::ref_ptr<osg::ShapeDrawable> xPointShapeDrawable = new osg::ShapeDrawable(
        new osg::Sphere(osg::Vec3f(distanceFromZero, 0, 0), sphereRadius));
    xPointShapeDrawable->setColor(getColor(255, 0, 0));
    coordinateSystemGeode->addDrawable(xPointShapeDrawable);

    osg::ref_ptr<osg::ShapeDrawable> yPointShapeDrawable = new osg::ShapeDrawable(
        new osg::Sphere(osg::Vec3f(0, distanceFromZero, 0), sphereRadius));
    yPointShapeDrawable->setColor(getColor(0, 255, 0));
    coordinateSystemGeode->addDrawable(yPointShapeDrawable);

    osg::ref_ptr<osg::ShapeDrawable> zPointShapeDrawable = new osg::ShapeDrawable(
        new osg::Sphere(osg::Vec3f(0, 0, distanceFromZero), sphereRadius));
    zPointShapeDrawable->setColor(getColor(0, 0, 255));
    coordinateSystemGeode->addDrawable(zPointShapeDrawable);

    return coordinateSystemGeode.release();
}
osg::ref_ptr<osg::Geode> createBall(osg::Vec3f position, float radius)
{
    osg::ref_ptr<osg::Geode> ballGeode = new osg::Geode;

    osg::ref_ptr<osg::ShapeDrawable> ballShapeDrawable = new osg::ShapeDrawable(
        new osg::Sphere(position, radius));
    ballShapeDrawable->setColor(getColor(255, 0, 0));
    ballGeode->addDrawable(ballShapeDrawable);

    return ballGeode.release();
}