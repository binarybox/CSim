.PHONY : simulator
simulator: library
	g++ -O1 -std=c++0x cache-sim/bin/Main.cpp -o cSim

library: simulator
	make -C cache-sim/
