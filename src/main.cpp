#include <raylib.h>

int main() {
	InitWindow(800, 600, "Interschem");

	while (!WindowShouldClose()) {
		// update data

		BeginDrawing();
		ClearBackground(RAYWHITE);
		// render on screen

		EndDrawing();
	}

	CloseWindow();
	return 0;
}