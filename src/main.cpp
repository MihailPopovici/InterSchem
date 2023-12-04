#include <raylib.h>

#include "codeComponents.h"

int main() {
	InitWindow(800, 600, "Interschem");

	StartNode* startNode = NewStartNode();
	SetLabel(startNode, "START");
	startNode->x = 5;
	startNode->y = 5;
	startNode->width = 100;
	startNode->height = 20;

	while (!WindowShouldClose()) {
		// update data

		BeginDrawing();
		ClearBackground(RAYWHITE);
		// render on screen

		DrawRectangle(startNode->x, startNode->y, startNode->width, startNode->height, GRAY);
		DrawText(startNode->label, startNode->x, startNode->y, 20.0f, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}