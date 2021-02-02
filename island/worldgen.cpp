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
const static int WIDTH = 2048;
const static int HEIGHT = 2048;
const static int OCTAVES = 8;
const static double PERSISTENCE = 0.55;
const static double LACUNARITY = 2.0;
const static double FREQUENCY = 0.25;
static int OFFSET_X, OFFSET_Y;

static unsigned char color[3];

const static int WATER = 85;
const static int BEACH = 88;
const static int MOUNTAINS = 140;
const static int SNOW = 210;

double data[HEIGHT][WIDTH] = { { 0 } };
double reef_data[HEIGHT][WIDTH] = { { 0 } };
double circle_gradient[HEIGHT][WIDTH] = { { 0 } };
double reef_circle_gradient[HEIGHT][WIDTH] = { { 0 } };
// }}}
// {{{ Function Declarations
template <class T>
void setColor(T, T, T);
template <class T>
void setColor(T);
void createCircleGradients();
// }}}
// {{{ main
int main(){
	srand(time(NULL));
	OFFSET_X = rand() % 10000000;
	OFFSET_Y = rand() % 10000000;
	// CImg INIT
	CImg<unsigned char> img(WIDTH,HEIGHT,1,3);

	// LIBNOISE INIT
	module::Perlin pnoise;
	pnoise.SetOctaveCount(OCTAVES);
	pnoise.SetPersistence(PERSISTENCE);
	pnoise.SetLacunarity(LACUNARITY);
	pnoise.SetFrequency(FREQUENCY);
	pnoise.SetSeed(time(NULL));
	
	// fill data with perlin noise
	double temp_data;
	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			temp_data = pnoise.GetValue((double)j / SCALE, (double)i / SCALE, 0.0);
			if (temp_data < -1.0) temp_data = -1.0;
			else if (temp_data > 1.0) temp_data = 1.0;
			data[i][j] = temp_data * 127.0 + 128.0;
			temp_data = pnoise.GetValue((double)(OFFSET_X + j) / SCALE, (double)(OFFSET_Y + i) / SCALE, 0.0);
			if (temp_data < -1.0) temp_data = -1.0;
			else if (temp_data > 1.0) temp_data = 1.0;
			reef_data[i][j] = temp_data * 127.0 + 128.0;
		}
	}

	createCircleGradients();

	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			if (data[i][j] < WATER){
				if (reef_data[i][j] > 50.0){
					setColor(0, 0, std::min((int)reef_data[i][j] + 70, 255));
				} else {
					setColor(0, 0, 100);
				}
			}
			else if (data[i][j] < BEACH) setColor(215, 200, 105);
			else if (data[i][j] > SNOW) setColor((int)data[i][j],(int)data[i][j],(int)data[i][j]);
			else if (data[i][j] > MOUNTAINS) setColor((int)data[i][j]-50,(int)data[i][j]-50,(int)data[i][j]-50);
			else setColor(0, (int)data[i][j], 0);
			img.draw_point(i, j, 0, color, 1);
		}
	}

	//img.display("perlin noise");
	img.save_jpeg("1.jpg");

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
void createCircleGradients(){
	int center_x = WIDTH / 2;
	int center_y = HEIGHT / 2;

	double dist_x;
	double dist_y;

	double reef_max_dist = 0.0;

	double dist, max_dist = 0.0;
	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			dist_x = std::abs(j - center_x);
			dist_y = std::abs(i - center_y);
			dist = std::sqrt(dist_x*dist_x+dist_y*dist_y);
			circle_gradient[i][j] = dist;
			if (dist > max_dist)
				max_dist = dist;
		}
	}
	
	double max_val = 0.0, reef_max_val = 0.0;
	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			circle_gradient[i][j] /= max_dist;
			reef_circle_gradient[i][j] = circle_gradient[i][j];
			circle_gradient[i][j] -= 0.5;
			circle_gradient[i][j] *= -2.0;

			reef_circle_gradient[i][j] -= 0.6;
			reef_circle_gradient[i][j] *= -2.0;

			if (circle_gradient[i][j] > 0)
				circle_gradient[i][j] *= 20.0;
			if (circle_gradient[i][j] > max_val)
				max_val = circle_gradient[i][j];
			
			if (reef_circle_gradient[i][j] > 0)
				reef_circle_gradient[i][j] *= 20.0;
			if (reef_circle_gradient[i][j] > reef_max_val)
				reef_max_val = reef_circle_gradient[i][j];
		}
	}
	
	max_dist = 0.0;
	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			circle_gradient[i][j] /= max_val;
			circle_gradient[i][j] *= data[i][j];

			reef_circle_gradient[i][j] /= reef_max_val;
			reef_circle_gradient[i][j] *= reef_data[i][j];

			if (circle_gradient[i][j] > 0)
				circle_gradient[i][j] *= 20.0;
			if (circle_gradient[i][j] > max_dist)
				max_dist = circle_gradient[i][j];

			if (reef_circle_gradient[i][j] > 0)
				reef_circle_gradient[i][j] *= 20.0;
			if (reef_circle_gradient[i][j] > reef_max_dist)
				reef_max_dist = reef_circle_gradient[i][j];
		}
	}

	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			data[i][j] = 255.0 * (circle_gradient[i][j] / max_dist);
			reef_data[i][j] = 255.0 * (reef_circle_gradient[i][j] / reef_max_dist);
		}
	}

}
// }}}
