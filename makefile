simulator: library
	g++ -O1 -std=c++0x src/Main.cpp -o cSim

library: simulator
	make -C src/

.PHONY : simulator
