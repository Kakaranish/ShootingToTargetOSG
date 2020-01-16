#include "PointOfViewHandler.hpp"
#include "Player.hpp"

bool PointOfViewHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (ea.getEventType() != osgGA::GUIEventAdapter::KEYDOWN)
    {
        return false;
    }

    int key = ea.getUnmodifiedKey();
    if (key == osgGA::GUIEventAdapter::KEY_Up)
    {
        _player->getCannon()->skewBarrel(UP);
    }
    else if (key == osgGA::GUIEventAdapter::KEY_Right)
    {
        _player->getCannon()->move(RIGHT);
    }
    else if (key == osgGA::GUIEventAdapter::KEY_Down)
    {
        _player->getCannon()->skewBarrel(DOWN);
    }
    else if (key == osgGA::GUIEventAdapter::KEY_Left)
    {
        _player->getCannon()->move(LEFT);
    }
    else if (key == osgGA::GUIEventAdapter::KEY_Space)
    {
        _player->shoot();
    }
    else
    {
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