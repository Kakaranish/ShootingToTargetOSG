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
        std::cout << "Key up " << allowEventFocus << std::endl;
        _player->getCannon()->rotate(1);
    }
    else if (key == osgGA::GUIEventAdapter::KEY_Right)
    {
        std::cout << "Key right" << std::endl;
        _player->moveCannon(1); //temp
        // osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
        // apcb->setAnimationPath(createAnimationPath(2, 1));
        // _player->getCannon()->getMatrixTransform()->setUpdateCallback(apcb.get());
    }
    else if (key == osgGA::GUIEventAdapter::KEY_Down)
    {
        std::cout << "Key down" << std::endl;
        _player->getCannon()->rotate(-1);
    }
    else if (key == osgGA::GUIEventAdapter::KEY_Left)
    {
        std::cout << "Key left" << std::endl;
        _player->moveCannon(-1); // temp
    }
    else if (key == osgGA::GUIEventAdapter::KEY_Space)
    {
        std::cout << "Space" << std::endl;
        // _player->showElapsedTime();
        _player->shoot();
    }
    else
    {
        return false;
    }

    return true;
}

osg::ref_ptr<osg::AnimationPath> PointOfViewHandler::createAnimationPath(float time, float movement)
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);

    unsigned numSamples = 100;
    float deltaTime = time / (float)numSamples;
    float deltaMovement = movement / (float)numSamples;

    for (size_t i = 0; i < numSamples; i++)
    {
        osg::AnimationPath::ControlPoint pos(osg::Vec3f(i * deltaMovement, 0.f, 0.f));
        path->insert(deltaTime * (float)i, pos);
    }

    return path.release();
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