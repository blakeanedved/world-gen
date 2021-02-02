worldgen:worldgen.cpp
	g++ -o ./worldgen worldgen.cpp -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -I/usr/local/include -L/usr/local/lib -Lnoise/lib/ -lnoise

noshow:worldgen.cpp
	g++ -o ./worldgen worldgen.cpp -Lnoise/lib/ -lnoise

all:worldgen
