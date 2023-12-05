#include <raylib.h>

#include "codeComponents.h"
#include "uiComponents.h"

int main() {
	InitWindow(800, 600, "Interschem");

	Button* test = NewButton();
	SetButtonLabel(test, "TEST", 24, 5);
	SetButtonColors(test, GREEN, RED);
	SetButtonPosition(test, 10, 10);

	while (!WindowShouldClose()) {
		// update data
		if (IsKeyPressed(KEY_SPACE)) {
			SetButtonPosition(test, GetMouseX(), GetMouseY());
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		// render on screen

		DrawButton(test);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}