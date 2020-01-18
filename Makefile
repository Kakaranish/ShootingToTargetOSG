# Makefile

main:
	g++ -std=c++11 -pedantic -Wall -Wl,--as-needed main.cpp PointOfViewHandler.cpp Player.cpp Cannon.cpp Utility.cpp Ball.cpp ShootingTarget.cpp World.cpp -losg -losgGA -losgSim -losgText -losgUtil -losgManipulator -losgViewer -losgAnimation -lOpenThreads  -o program

clean:
	rm -f program
