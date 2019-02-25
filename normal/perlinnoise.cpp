// {{{ Includes
#include <iostream>
#include "noise/noise.h"
#include "CImg.h"
#include <time.h>
// }}}
// {{{ Globals
static double SCALE;
static int WIDTH;
static int HEIGHT;
static int OCTAVES;
static double PERSISTENCE;
static double LACUNARITY;
static double FREQUENCY;

static unsigned char color[3];
// }}}
// {{{ Function Declarations
template <class T>
void setColor(T, T, T);
template <class T>
void setColor(T);
// }}}
// {{{ main()
int main(){

	std::string garbage;
	std::cin >> garbage >> SCALE >> garbage >> WIDTH >> garbage >> HEIGHT >> garbage >> OCTAVES >> garbage >> PERSISTENCE >> garbage >> LACUNARITY >> garbage >> FREQUENCY;

	cimg_library::CImg<unsigned char> img(WIDTH, HEIGHT, 1, 3);

	noise::module::Perlin pnoise;

	pnoise.SetOctaveCount(OCTAVES);
	pnoise.SetPersistence(PERSISTENCE);
	pnoise.SetLacunarity(LACUNARITY);
	pnoise.SetFrequency(FREQUENCY);
	pnoise.SetSeed(time(NULL));

	double value;
	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			value = pnoise.GetValue(j / SCALE, i / SCALE, 0.0);

			if (value > 1) value = 1;
			else if (value < -1) value = -1;

			value *= 127.0;
			value += 128.0;

			setColor(value);
			img.draw_point(j, i, 0, color, 1);
		}
	}

	img.display("Perlin Noise");

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
