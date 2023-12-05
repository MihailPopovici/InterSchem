#include <raylib.h>

#include "codeComponents.h"
#include "uiComponents.h"

int main() {
	InitWindow(800, 600, "Interschem");

	Button* test = NewButton();
	SetButtonLabel(test, "TEST", 24, 5);
	SetButtonColors(test, GREEN, WHITE);
	SetButtonPosition(test, 10, 10);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// update data
		
		if (IsButtonClicked(test)) {
			OnButtonClick(test);
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