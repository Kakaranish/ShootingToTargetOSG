#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>
#include "PointOfViewHandler.hpp"
#include "Player.hpp"
#include "Utility.hpp"
#include "World.hpp"


void showHelpMessage()
{
    std::cout << "Synopsis: " << std::endl;
    std::cout << "      ./main [option]" << std::endl;
    std::cout << "Available options: " << std::endl;
    std::cout << "-h    shows help" << std::endl;
    std::cout << "-l    set locked view" << std::endl;
}

int main(int argc, char const *argv[])
{
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

    World world;
    osg::ref_ptr<osg::Group> root = world.getRoot();

    Player *player = new Player(viewer, &world);
    viewer->addEventHandler(player->getPointOfViewHandler());

    viewer->setUpViewInWindow(100, 100, 800, 600);
    viewer->setSceneData(root.get());

    std::cout << argc << std::endl;
    if (argc == 1)
    {
        return viewer->run();
    }
    else if (argc == 2)
    {
        std::string arg = argv[1];
        if (arg == "-h")
        {
            showHelpMessage();
            return 0;
        }
        else if (arg == "-l")
        {
            const float camYDistance = -28;
            viewer->getCamera()->setViewMatrix(osg::Matrix::lookAt(
                osg::Vec3(0, camYDistance, 3.5),
                osg::Vec3(0, 0, 3),
                osg::Vec3(0, -1, 1)));
            viewer->setRunFrameScheme(osgViewer::Viewer::ON_DEMAND);

            viewer->realize();
            while (!viewer->done())
            {
                viewer->frame();
            }
        }
        else
        {
            std::cout << "ERROR: Invalid execution syntax. " << std::endl;
            showHelpMessage();
            return -1;
        }
    }
    else
    {
        std::cout << "ERROR: Invalid execution syntax. " << std::endl;
        showHelpMessage();
        return -1;
    }
}
