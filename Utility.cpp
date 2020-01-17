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