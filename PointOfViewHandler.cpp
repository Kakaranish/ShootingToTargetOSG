#include "PointOfViewHandler.hpp"
#include "Player.hpp"

bool PointOfViewHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (ea.getEventType() != osgGA::GUIEventAdapter::KEYDOWN)
    {
        return false;
    }

    switch (ea.getUnmodifiedKey())
    {
    case osgGA::GUIEventAdapter::KEY_Up:
        std::cout << "Key up " << allowEventFocus << std::endl;
        break;
    case osgGA::GUIEventAdapter::KEY_Right:
        std::cout << "Key right" << std::endl;
        break;
    case osgGA::GUIEventAdapter::KEY_Down:
        std::cout << "Key down" << std::endl;
        break;
    case osgGA::GUIEventAdapter::KEY_Left:
        std::cout << "Key left" << std::endl;
        break;
    case osgGA::GUIEventAdapter::KEY_Space:
        std::cout << "Space" << std::endl;
        break;
    default:
        return false;
    }

    return true;
}

PointOfViewHandler::PointOfViewHandler(const osg::ref_ptr<osgViewer::Viewer> &viewer)
{
    _viewer = viewer;
    allowEventFocus = true;

    _viewer->getCamera()->setAllowEventFocus(allowEventFocus);
}

void PointOfViewHandler::setPlayer(Player *player)
{
    _player = player;
}