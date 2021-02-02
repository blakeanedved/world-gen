// {{{ Defines
#define cimg_display 0
#include <iostream>
#include "CImg.h"
#include "noise/noise.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>

using namespace cimg_library;
using namespace noise;
// }}}
// {{{ Globals
const static double SCALE = 300.0;
const static int WIDTH = 800;
const static int HEIGHT = 800;
const static int OCTAVES = 8;
const static double PERSISTENCE = 0.55;
const static double LACUNARITY = 2.0;
const static double FREQUENCY = 2.0;
const static double WORM_SPEED = 8.0;
const static int WORM_MAX_LENGTH = 200;

//int data[HEIGHT][WIDTH] = { { 1 } };

static unsigned char color[3];
// }}}
// {{{ Function Declarations
template <class T>
void setColor(T, T, T);
template <class T>
void setColor(T);
// }}}
// {{{ main
int main(){
	srand(time(NULL));

	CImg<unsigned char> img(WIDTH*2, HEIGHT, 1, 3);

	module::Perlin pnoise;
	pnoise.SetOctaveCount(OCTAVES);
	pnoise.SetPersistence(PERSISTENCE);
	pnoise.SetLacunarity(LACUNARITY);
	pnoise.SetFrequency(FREQUENCY);
	pnoise.SetSeed(time(NULL));
	double weight = 0.25;
	double x[] = {0.0, 5000.0, 10000.0};
	double y[] = {0.0, 5000.0, 10000.0};
	double z[] = {0.0, 5000.0, 10000.0};
	int worm_x[] = {WIDTH - WIDTH / 6, WIDTH, WIDTH + WIDTH / 6};
	int worm_y[] = {0, 0, 0};
	for (int i = 0; i < WORM_MAX_LENGTH; i++){
		for (int j = 0; j < 3; j++){
			double dir = pnoise.GetValue(x[j] / SCALE, y[j] / SCALE, z[j] / SCALE);
			if (dir < -1) dir = -1;
			else if (dir > 1) dir = 1;
			dir = ((dir / (0.5 / 0.3)) + 1.5);
			//std::cout << dir << " to ";
			if (dir < 1.4){
				if (dir > 1.1) dir = std::max(1.1, dir + (1.1 - dir) * weight);
			} else if (dir > 1.6){
				if (dir < 1.9) dir = std::min(1.9, dir + (1.9 - dir) * weight);
			}
			//std::cout << dir << std::endl;
			dir *= M_PI;
			worm_x[j] += floor(WORM_SPEED * cos(dir));
			worm_y[j] -= floor(WORM_SPEED * sin(dir));
			if (j == 0)
				setColor(255, 0, 0);
			if (j == 1)
				setColor(0, 255, 0);
			if (j == 2)
				setColor(0, 0, 255);
			img.draw_rectangle(worm_x[j]-1, worm_y[j]-1, worm_x[j]+1, worm_y[j]+1, color, 1);
			x[j]++;
			y[j]++;
		}
	}
	
	//img.display("");
	img.save_jpeg("worms.jpg");

	return 0;
}
// }}}
// {{{ Function Definitions
template <class T>
void setColor(T r, T g, T b){
	color[0] = (unsigned char)r;
	color[1] = (unsigned char)g;
	color[2] = (unsigned char)b;
}
template <class T>
void setColor(T c){
	color[0] = (unsigned char)c;
	color[1] = (unsigned char)c;
	color[2] = (unsigned char)c;
}
// }}}
