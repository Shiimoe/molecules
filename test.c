#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define BALLS 3
#define ballRadius 10
#define SPEED 10

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "hot and sweaty");

	// Vector2 Positions[BALLS];
	// Vector2 Velocities[BALLS];
	// for (int i=0; i < BALLS; i++) {
	// 	Positions[i] = (Vector2){(rand() % screenWidth - ballRadius) + ballRadius*2, (rand() % screenHeight-ballRadius) + ballRadius*2};
	// 	Velocities[i] = (Vector2){(rand() % SPEED) - (rand() % SPEED), (rand() % SPEED) - (rand() % SPEED)};
	// }

	Vector2 Positions[BALLS] = {{100, 118}, {150, 100}, {700, 100}};
	Vector2 Velocities[BALLS] = {{3, 0}, {0, 0}, {0, 0}};

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		// Updating variables

		for (int i=0; i < BALLS; i++) {
			Vector2 position = Positions[i];
			Vector2 velocity = Velocities[i];
			Positions[i].x += Velocities[i].x;
			Positions[i].y += Velocities[i].y;


			if (Positions[i].x >= screenWidth - ballRadius) {
				Velocities[i].x = -Velocities[i].x;
				Positions[i].x = screenWidth - ballRadius;
			} else if (Positions[i].x <= 0 + ballRadius) {
				Velocities[i].x = -Velocities[i].x;
				Positions[i].x = ballRadius;
			}

			if (Positions[i].y >= screenHeight - ballRadius) {
				Velocities[i].y = -Velocities[i].y;
				Positions[i].y = screenHeight - ballRadius;
			} else if (Positions[i].y <= 0 + ballRadius) {
				Velocities[i].y = -Velocities[i].y;
				Positions[i].y = ballRadius;
			}

			for (int j=i+1; j < BALLS; j++){
				if (sqrt(pow(Positions[i].x - Positions[j].x, 2) + pow(Positions[i].y - Positions[j].y, 2)) <= 2 * ballRadius) {
					
					//manual calculation of non-head on elastic collisions 
					// float dPx = Positions[i].x - Positions[j].x;
					// float dPy = Positions[i].y - Positions[j].y;
					// float dVx = Velocities[i].x - Velocities[j].x;
					// float dVy = Velocities[i].y - Velocities[j].y;
					// // float norm = abs(pow(dPx) + pow(dPy, 2));
					// float norm = abs(pow(dPx, 2) + pow(dPy, 2));
					// float dot = dVx * dPx + dVy * dPy;

					// Velocities[i].x = Velocities[i].x - ((dot/norm) * dPx); 
					// Velocities[i].y = Velocities[i].y - ((dot/norm) * dPy); 

					// Velocities[j].x = Velocities[j].x + ((dot/norm) * dPx);
					// Velocities[j].y = Velocities[j].y + ((dot/norm) * dPy); 
					///////////////////////////////////////////////////////

					float dot = Vector2DotProduct(Vector2Subtract(Velocities[i], Velocities[j]), Vector2Subtract(Positions[i], Positions[j]));
					float normSquared = Vector2LengthSqr(Vector2Subtract(Positions[i], Positions[j]));

					Velocities[i] = Vector2Subtract(Velocities[i], Vector2Scale(Vector2Subtract(Positions[i], Positions[j]), dot/normSquared));
					Velocities[j] = Vector2Subtract(Velocities[j], Vector2Scale(Vector2Subtract(Positions[i], Positions[j]), -dot/normSquared));


					// // moves balls out of each other, stops them from becoming stuck inside each other

					float dist = Vector2Distance(Positions[i], Positions[j]);

					if (Positions[i].x < Positions[j].x) {
						 Positions[i].x -= abs(ballRadius - dist/2);
						 Positions[j].x += abs(ballRadius - dist/2);
					}
					if (Positions[i].y < Positions[j].y) {
						 Positions[i].y -= abs(ballRadius - dist/2);
						 Positions[j].y += abs(ballRadius - dist/2);
					}
					if (Positions[i].x > Positions[j].x) {
						 Positions[i].x += abs(ballRadius - dist/2);
						 Positions[j].x -= abs(ballRadius - dist/2);
					}
					if (Positions[i].y > Positions[j].y) {
						 Positions[i].y += abs(ballRadius - dist/2);
						 Positions[j].y -= abs(ballRadius - dist/2);
					}

				}
			}

		}
		// Drawing to screen
		BeginDrawing(); // I wish this were wrapped in curly bwackets qwq

            ClearBackground(RAYWHITE);

			// DrawCircleV(ballPosition, ballRadius * 2, RED);
			double momentum = 0;
			for (int i=0; i < BALLS; i++) {
				if (i % 50 == 0) DrawCircleV(Positions[i], ballRadius, BLUE);
				else DrawCircleV(Positions[i], ballRadius, RED);
				momentum += sqrt(pow(Velocities[i].x, 2) + pow(Velocities[i].y, 2));
			}

			DrawText(TextFormat("%lf", momentum), 20, 20, 100, LIGHTGRAY);

		EndDrawing();
    }

    CloseWindow();

    return 0;
}
