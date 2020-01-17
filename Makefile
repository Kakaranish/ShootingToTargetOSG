# Makefile

main:
	g++ -std=c++98 -pedantic -Wall -Wl,--as-needed main.cpp PointOfViewHandler.cpp Player.cpp Cannon.cpp Utility.cpp Ball.cpp ShootingTarget.cpp World.cpp -losg -losgDB -losgFX -losgGA -losgParticle -losgSim -losgText -losgUtil -losgTerrain -losgManipulator -losgViewer -losgWidget -losgShadow -losgAnimation -losgVolume -lOpenThreads  -o program

clean:
	rm -f program
