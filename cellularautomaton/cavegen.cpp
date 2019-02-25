// {{{ Includes
#include <iostream>
#include "noise/noise.h"
#include <vector>
#include "CImg.h"
// }}}
// {{{ Globals
const static int WIDTH = 128;
const static int HEIGHT = 128;
const static int GENERATIONS = 15;
const static double CHANCE_TO_SPAWN = 0.45;
static std::vector<int> B = {5, 6, 7, 8};
static std::vector<int> S = {3, 4, 5, 6, 7, 8};
const static int SCALE = 8;
const static int OCTAVES = 5;
const static double PERSISTENCE = 0.55;
const static double LACUNARITY = 2.0;
const static double FREQUENCY = 1.2;

static unsigned char color[3];

static int DATA[HEIGHT][WIDTH];
static int NEW_DATA[HEIGHT][WIDTH];

noise::module::Perlin pnoise;
// }}}
// {{{ Function Declarations
template <class T>
void setColor(T, T, T);
template <class T>
void setColor(T);
// }}}
// {{{ main()
int main(){

	srand(time(NULL));

	cimg_library::CImg<unsigned char> img(SCALE * WIDTH, SCALE * HEIGHT, 1, 3);

	//pnoise.SetSeed(time(NULL));
	//pnoise.SetOctaveCount(OCTAVES);
	//pnoise.SetFrequency(FREQUENCY);
	//pnoise.SetLacunarity(LACUNARITY);
	//pnoise.SetPersistence(PERSISTENCE);

	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			DATA[i][j] = (double)(rand() % 100) / 100 <= CHANCE_TO_SPAWN ? 1 : 0;
		}
	}

	//double value;
	//double randomOffset = (rand() % 1000) / 10;
	//for (int i = 0; i < HEIGHT; i++){
		//value = pnoise.GetValue(randomOffset + ((double)i / 300.0), 100.0, 100.0);

		//if (value > 1) value = 1;
		//else if (value < -1) value = -1;
	
		//value *= (WIDTH / 2);
		//value += (WIDTH / 2);
		//std::cout << value << std::endl;
		//DATA[i][(int)value] = 0;
		//if ((int)value > 0)
			//DATA[i][(int)value-1] = 0;
		//if ((int)value <  WIDTH - 1)
			//DATA[i][(int)value+1] = 0;
	//}

	for (int i = 0; i < GENERATIONS; i++){
		for (int j = 0; j < HEIGHT; j++){
			for (int k = 0; k < WIDTH; k++){

				int num_alive = 0;
				
				for (int l = -1; l < 2; l++){
					for (int m = -1; m < 2; m++){
						if (l == 0 && m == 0)
							continue;
						
						int desX = k + m;
						int desY = j + l;

						if (desY >= 0 && desX >= 0 && desY < HEIGHT && desX < WIDTH && DATA[desY][desX] == 1)
							num_alive++;
					}
				}

				if (DATA[j][k] == 1){
					
					bool shouldDie = true;
					for (auto s : S){
						if (num_alive == s){
							shouldDie = false;
							break;
						}
					}

					NEW_DATA[j][k] = shouldDie ? 0 : 1;

				} else {

					bool shouldBeBorn = false;
					for (auto b : B){
						if (num_alive == b){
							shouldBeBorn = true;
							break;
						}
					}

					NEW_DATA[j][k] = shouldBeBorn ? 1 : 0;
				}
			}
		}

		for (int j = 0; j < HEIGHT; j++){
			for (int k = 0; k < WIDTH; k++){
				DATA[j][k] = NEW_DATA[j][k];
				NEW_DATA[j][k] = 0;
			}
		}
	}

	for (int i = 0; i < SCALE * HEIGHT; i+=SCALE){
		for (int j = 0; j < SCALE * WIDTH; j+=SCALE){
			setColor(DATA[i / SCALE][j / SCALE]*255);
			for (int k = 0; k < SCALE; k++){
				for (int l = 0; l < SCALE; l++){
					img.draw_point(j+k, i+l, 0, color, 1);
				}
			}
		}
	}

	img.display("Cellular Automata");

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
