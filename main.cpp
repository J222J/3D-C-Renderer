// execute with g++ main.cpp -lSDL2 -lSDL2main

#include<cmath>
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_timer.h>
#include<vector>

using namespace std;

int FOCAL_LENGTH = 50;
const int WINDOW_HEIGHT = 900;
const int WINDOW_WIDTH = 1600;

struct Coordinate3D {
	int x, y, z;

	Coordinate3D(int xx, int yy, int zz) {
		x = xx;
		y = yy;
		z = zz;
	}
};

struct Coordinate2D {
	int x, y;

	Coordinate2D(int xx, int yy) {
		x = xx;
		y = yy;
	}
};

void pixel(SDL_Renderer* renderer, int x, int y) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderDrawPoint(renderer, x, y);
}

// updated line function (faster and works better)
void line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int t) {
	int dx = x2-x1;
	int dy = y2-y1;

	int steps = max(abs(dx), abs(dy));
	double xstep = (double)dx/steps;
	double ystep = (double)dy/steps;

	double curx = (double)x1;
	double cury = (double)y1;

	while (steps--) {
		pixel(renderer, (int)curx, (int)cury);
		curx += xstep;
		cury += ystep;
	}
}

// Converts Coordinate3D into Coordinate2D
Coordinate2D C3DtoC2D(Coordinate3D coordinate) {
	int x2d, y2d;

	int x = coordinate.x;
	int y = coordinate.y;
	int z = coordinate.z;

	int f = FOCAL_LENGTH;

	x2d = (abs(x) * f) / (f + z) * x/abs(x);
	y2d = (abs(y) * f) / (f + z) * y/abs(y);

	Coordinate2D returnCoordinate(x2d, y2d);
	return returnCoordinate;
}

Coordinate3D VectorToC3D(vector<int> v) {
	Coordinate3D coordinates(v[0], v[1], v[2]);

	return coordinates;
}

vector<pair<Coordinate2D, Coordinate2D>> convertVertices(vector<pair<vector<int>, vector<int>>> vertices) {
	vector<pair<Coordinate2D, Coordinate2D>> ans;

	for (pair<vector<int>, vector<int>> e : vertices) {
		Coordinate2D A = C3DtoC2D(VectorToC3D(e.first));
		Coordinate2D B = C3DtoC2D(VectorToC3D(e.second));

		ans.push_back(make_pair(A, B));
	}

	return ans;
}

int main() {
	// All the useless shit

	SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    // Reset the scene (could be useful for animations)

    SDL_RenderClear(renderer);

    // Make the background white

    SDL_Rect backg;
    backg.x = 0;
    backg.y = 0;
    backg.h = WINDOW_HEIGHT;
    backg.w = WINDOW_WIDTH;

    // Draw

    vector<pair<vector<int>, vector<int>>> vertices = {{{100+200, 100, 200}, {200+200, 100, 200}},
													   {{100+200, 100, 200}, {100+200, 200, 200}},
													   {{200+200, 100, 200}, {200+200, 200, 200}},
													   {{100+200, 200, 200}, {200+200, 200, 200}},

													   {{100+200, 100, 205}, {200+200, 100, 205}},
													   {{100+200, 100, 205}, {100+200, 200, 205}},
													   {{200+200, 100, 205}, {200+200, 200, 205}},
													   {{100+200, 200, 205}, {200+200, 200, 205}},

													   {{100+200, 100, 200}, {100+200, 100, 205}},
													   {{200+200, 100, 200}, {200+200, 100, 205}},
													   {{100+200, 200, 200}, {100+200, 200, 205}},
													   {{200+200, 200, 200}, {200+200, 200, 205}}};
	
    while (true) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
	    	break;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &backg);
		
		bool dox = true;

		for (pair<vector<int>, vector<int>> e : vertices) {
			Coordinate2D A = C3DtoC2D(VectorToC3D(e.first));
			Coordinate2D B = C3DtoC2D(VectorToC3D(e.second));

			line(renderer, A.x, A.y, B.x, B.y, 4);
			if (A.x >= WINDOW_WIDTH-100 or B.x >= WINDOW_WIDTH-100 or A.y >= WINDOW_HEIGHT-100 or B.y >= WINDOW_HEIGHT-100)
				dox = false;
		}

		for (pair<vector<int>, vector<int>> &e : vertices)
			if (dox) {
				e.first[2]--;
				e.second[2]--;
			}
		if (!dox)
			break;

		SDL_RenderPresent(renderer);
		SDL_Delay(1000/60);
    }

    int add = 1;
    while (true) {
    	if (SDL_PollEvent(&event) and event.type == SDL_QUIT)
    		break;

    	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    	SDL_RenderFillRect(renderer, &backg);

    	for (pair<vector<int>, vector<int>> e : vertices) {
    		Coordinate2D A = C3DtoC2D(VectorToC3D(e.first));
			Coordinate2D B = C3DtoC2D(VectorToC3D(e.second));

			line(renderer, A.x, A.y, B.x, B.y, 4);
    	}

    	FOCAL_LENGTH += add;
    	if (FOCAL_LENGTH >= 150)
    		add = -1;
    	if (FOCAL_LENGTH <= 50)
    		add = 1;

    	SDL_RenderPresent(renderer);
    	SDL_Delay(1000/60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
