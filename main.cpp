// execute with g++ main.cpp -lSDL2 -lSDL2main

#include<cmath>
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<vector>

using namespace std;

#define FOCAL_LENGTH 50
#define WINDOW_WIDTH 900

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

void line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int t) {
	int r = 1;
	if (t == 1)
		r = 0;

	if (x1 == x2) {
		if (y1 > y2) {
			swap(x1, x2);
			swap(y1, y2);
		}

		for (int y = y1; y <= y2; y++) {
			for (int a = x1-t/2; a <= x1+t/2; a++)
				for (int b = y-t/2; b <= y+t/2-r; b++)
					pixel(renderer, a, b);
		}
	} else {
		if (x1 > x2) {
			swap(x1, x2);
			swap(y1, y2);
		}

		if (y1 <= y2) {
			int dx = x2-x1;
			int dy = y2-y1;
		 
			int x = x1;
			int y = y1;
		 
			int p = 2*dy-dx;
		 
			while(x < x2) {
				if(p >= 0) {
					for (int a = x-t/2; a <= x+t/2; a++)
						for (int b = y-t/2; b <= y+t/2-r; b++)
							pixel(renderer, a, b);
					y++;
					p += 2*dy-2*dx;
				} else {
					for (int a = x-t/2; a <= x+t/2; a++)
						for (int b = y-t/2; b <= y+t/2-r; b++)
							pixel(renderer, a, b);
					p += 2*dy;
				}
				x++;
			}
		} else {
			int dx = x2-x1;
			int dy = y1-y2;
		 
			int x = x2;
			int y = y2;
		 
			int p = 2*dy-dx;
		 
			while(x > x1) {
				if(p >= 0) {
					pixel(renderer, x, y);
					y++;
					p += 2*dy-2*dx;
				} else {
					pixel(renderer, x, y);
					p += 2*dy;
				}
				x--;
			}
		}
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
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);

    // Reset the scene (could be useful for animations)

    SDL_RenderClear(renderer);

    // Make the background white

    SDL_Rect backg;
    backg.x = 0;
    backg.y = 0;
    backg.h = WINDOW_WIDTH;
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
	
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
	    	break;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &backg);
		
		bool dox = true;

		for (pair<vector<int>, vector<int>> e : vertices) {
			Coordinate2D A = C3DtoC2D(VectorToC3D(e.first));
			Coordinate2D B = C3DtoC2D(VectorToC3D(e.second));

			line(renderer, A.x, A.y, B.x, B.y, 4);
			if (A.x >= WINDOW_WIDTH-10 or B.x >= WINDOW_WIDTH-10 or A.y >= WINDOW_WIDTH-10 or B.y >= WINDOW_WIDTH-10)
				dox = false;
		}

		for (pair<vector<int>, vector<int>> &e : vertices)
			if (dox) {
				e.first[2]--;
				e.second[2]--;
			}

		SDL_RenderPresent(renderer);
		SDL_Delay(1000/120);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}