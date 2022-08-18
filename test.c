#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define BALLS 500
#define ballRadius 10
#define SPEED 15
#define POPULATION_SIZE SPEED + 10

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "hot and sweaty");

	Vector2 Positions[BALLS];
	Vector2 Velocities[BALLS];
	for (int i=0; i < BALLS; i++) {
		Positions[i] = (Vector2){(rand() % screenWidth - ballRadius) + ballRadius*2, (rand() % screenHeight-ballRadius) + ballRadius*2};
		Velocities[i] = (Vector2){(rand() % SPEED) - (rand() % SPEED), (rand() % SPEED) - (rand() % SPEED)};
	}

	// Vector2 Positions[BALLS] = {{600, 600}, {300, 600}, {900, 600}, {600, 900}, {600, 300}, {900, 900}, {300, 300}, {300, 900}, {900,300}};
	// Vector2 Velocities[BALLS] = {{10, 0}, {2, 0}, {-2, 0}, {0, -2}, {0, 2}, {-2, -2}, {2, 2}, {2, -2}, {-2, 2}};

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
		}
		for (int i = 0; i < BALLS; i++) {

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
				if (sqrtf(powf(Positions[i].x - Positions[j].x, 2) + powf(Positions[i].y - Positions[j].y, 2)) <= 2 * ballRadius) {
					// moving molecules so they don't overlap on collision 
					
					float ratio = Vector2Length(Velocities[i])/(Vector2Length(Velocities[i]) + Vector2Length(Velocities[j]));

					Vector2 diff_ij = Vector2Subtract(Positions[i], Positions[j]);

					Vector2 dP = Vector2Scale(Vector2Scale(diff_ij, 1 / (Vector2Length(diff_ij))), (2 * ballRadius - Vector2Length(diff_ij)));
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
					float dot = Vector2DotProduct(Vector2Subtract(Velocities[i], Velocities[j]), Vector2Subtract(Positions[i], Positions[j]));
					float normSquared = Vector2LengthSqr(Vector2Subtract(Positions[i], Positions[j]));

					Velocities[i] = Vector2Subtract(Velocities[i], Vector2Scale(Vector2Subtract(Positions[i], Positions[j]), dot/normSquared));
					Velocities[j] = Vector2Subtract(Velocities[j], Vector2Scale(Vector2Subtract(Positions[i], Positions[j]), -dot/normSquared));
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
					if (j < sqrtf(Vector2LengthSqr(Velocities[i])) && sqrtf(Vector2LengthSqr(Velocities[i])) < j + 1) { 
						population[(int)j/1] += 1;
					}
				}
			}
			
			for (int i = 0; i < POPULATION_SIZE; i++) {
				DrawLineEx((Vector2){10 + (i*10), screenHeight-10}, (Vector2){10 + (i*10), (screenHeight - 12 - (population[i] * 20 / log(BALLS)))}, 5, BLACK);
				population[i] = 0;
			}
			momentum = sqrtf(tot_x + tot_y);
			DrawText(TextFormat("%.3f", momentum), 20, 20, 100, GRAY);


		EndDrawing();
    }

    CloseWindow();

    return 0;
}
