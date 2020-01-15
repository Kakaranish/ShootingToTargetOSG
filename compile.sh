#!/bin/bash

g++  -std=c++98 -pedantic -Wall   -Wl,--as-needed   osg_misc1.cpp PointOfViewHandler.cpp Player.cpp -losg -losgDB -losgFX -losgGA -losgParticle -losgSim -losgText -losgUtil -losgTerrain -losgManipulator -losgViewer -losgWidget -losgShadow -losgAnimation -losgVolume -lOpenThreads  -o osg_misc1