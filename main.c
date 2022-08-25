#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define vec2sub Vector2Subtract
#define vec2len Vector2Length
#define vec2scl Vector2Scale
#define vec2lensqr Vector2LengthSqr
#define vec2 Vector2
#define vec2dot Vector2DotProduct
#define scrnH 600
#define scrnW 800

// Maybe make some enum int global variables?

enum {
	BALLS = 150,
	ballRadius = 8,
	SPEED = 10,
	POPULATION_SIZE = (SPEED + 10),
};


void randBalls(vec2 Positions[], vec2 Velocities[]) {
	for (int i=0; i < BALLS; i++) {
		Positions[i] = (vec2){(rand() % scrnW - ballRadius) + ballRadius*2, (rand() % scrnH-ballRadius) + ballRadius*2};
		Velocities[i] = (vec2){(rand() % SPEED) - (rand() % SPEED), (rand() % SPEED) - (rand() % SPEED)};
	}
}


int main(void)
{
    //const int screenWidth = 800;
    //const int screenHeight = 600;

    InitWindow(scrnW, scrnH, "hot and sweaty");

	vec2 Positions[BALLS];
	vec2 Velocities[BALLS];
	randBalls(Positions, Velocities);

	// vec2 Positions[BALLS] = {{600, 600}, {300, 600}, {900, 600}, {600, 900}, {600, 300}, {900, 900}, {300, 300}, {300, 900}, {900,300}};
	// vec2 Velocities[BALLS] = {{10, 0}, {2, 0}, {-2, 0}, {0, -2}, {0, 2}, {-2, -2}, {2, 2}, {2, -2}, {-2, 2}};

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		// Updating variables

		if (IsKeyDown(KEY_R)) randBalls(Positions, Velocities);  

		for (int i=0; i < BALLS; i++) {
			vec2 position = Positions[i];
			vec2 velocity = Velocities[i];
			Positions[i].x += Velocities[i].x;
			Positions[i].y += Velocities[i].y;
		}
		for (int i = 0; i < BALLS; i++) {

			if (Positions[i].x >= scrnW - ballRadius) {
				Velocities[i].x = -Velocities[i].x;
				Positions[i].x = scrnW - ballRadius;
			} else if (Positions[i].x <= 0 + ballRadius) {
				Velocities[i].x = -Velocities[i].x;
				Positions[i].x = ballRadius;
			}

			if (Positions[i].y >= scrnH - ballRadius) {
				Velocities[i].y = -Velocities[i].y;
				Positions[i].y = scrnH - ballRadius;
			} else if (Positions[i].y <= 0 + ballRadius) {
				Velocities[i].y = -Velocities[i].y;
				Positions[i].y = ballRadius;
			}

			for (int j=i+1; j < BALLS; j++){
				if (sqrtf(powf(Positions[i].x - Positions[j].x, 2) + powf(Positions[i].y - Positions[j].y, 2)) <= 2 * ballRadius) {
					// moving molecules so they don't overlap on collision 
					
					float ratio = vec2len(Velocities[i])/(vec2len(Velocities[i]) + vec2len(Velocities[j]));

					vec2 diff_ij = vec2sub(Positions[i], Positions[j]);

					vec2 dP = vec2scl(vec2scl(diff_ij, 1 / (vec2len(diff_ij))), (2 * ballRadius - vec2len(diff_ij)));
					dP.x = fabs(dP.x);
					dP.y = fabs(dP.y);

					if (Positions[i].x  > Positions[j].x) {
						Positions[i].x += dP.x * ratio;
						Positions[j].x -= dP.x * (1 - ratio);
					}
					else if (Positions[i].x < Positions[j].x) {
						 Positions[i].x -= dP.x * ratio;
						 Positions[j].x +=  dP.x * (1 - ratio);
					}

					if (Positions[i].y > Positions[j].y) {
						 Positions[i].y += dP.y * ratio;
						 Positions[j].y -= dP.y * (1 - ratio);
					}
					else if (Positions[i].y < Positions[j].y) {
						 Positions[i].y -= dP.y * ratio;
						 Positions[j].y += dP.y * (1 - ratio);
					}
					
					
					// calculating new velocity
					float dot = vec2dot(vec2sub(Velocities[i], Velocities[j]), vec2sub(Positions[i], Positions[j]));
					float normSquared = vec2lensqr(vec2sub(Positions[i], Positions[j]));

					Velocities[i] = vec2sub(Velocities[i], vec2scl(vec2sub(Positions[i], Positions[j]), dot/normSquared));
					Velocities[j] = vec2sub(Velocities[j], vec2scl(vec2sub(Positions[i], Positions[j]), -dot/normSquared));
				}
			}

		}
		// Drawing to screen
		BeginDrawing(); // I wish this were wrapped in curly bwackets qwq

            ClearBackground(RAYWHITE);
			int population[POPULATION_SIZE];

			// DrawCircleV(ballPosition, ballRadius * 2, RED);
			float momentum = 0;
			float tot_x = 0;
			float tot_y = 0;
			for (int i=0; i < BALLS; i++) {
				if (i % 50 == 0) DrawCircleV(Positions[i], ballRadius, BLUE);
				else DrawCircleV(Positions[i], ballRadius, RED);
				tot_x += powf(Velocities[i].x, 2);
				tot_y += powf(Velocities[i].y, 2);
				for (int j = 0; j < POPULATION_SIZE; j += 1) {
					if (j < sqrtf(vec2lensqr(Velocities[i])) && sqrtf(vec2lensqr(Velocities[i])) < j + 1) { 
						population[(int)j/1] += 1;
					}
				}
			}
			
			for (int i = 0; i < POPULATION_SIZE; i++) {
				DrawLineEx((vec2){10 + (i*10), scrnH-10}, (vec2){10 + (i*10), (scrnH - 12 - (population[i] * 20 / log(BALLS)))}, 5, BLACK);
				population[i] = 0;
			}
			momentum = sqrtf(tot_x + tot_y);
			//DrawText(TextFormat("%.3f", momentum), 20, 20, 100, GRAY);


		EndDrawing();
    }

    CloseWindow();

    return 0;
}
