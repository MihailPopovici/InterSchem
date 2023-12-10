#include <raylib.h>

#include "codeComponents.h"
#include "uiComponents.h"

int main() {
	InitWindow(1020, 800, "Interschem");

	unsigned globalID = 0;

	StartNode* start = NewStartNode(globalID);
	SetStartNodePosition(start, 100, 100);
	SetStartNodeSize(start, 5, 20);

	StartNode* start2 = NewStartNode(globalID);
	SetStartNodePosition(start2, 500, 50);
	SetStartNodeSize(start2, 5, 20);

	StopNode* stop = NewStopNode(globalID);
	SetStopNodePosition(stop, 300, 300);
	SetStopNodeSize(stop, 5, 20);

	StopNode* stop2 = NewStopNode(globalID);
	SetStopNodePosition(stop2, 5, 200);
	SetStopNodeSize(stop2, 5, 20);

	unsigned nLinks = 0;
	Link links[16];
	links[0] = NewLink(start, stop, nLinks);
	links[1] = NewLink(start2, stop2, nLinks);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// update data

		BeginDrawing();
		ClearBackground(BLACK);
		// render on screen

		DrawStartNode(start);
		DrawStartNode(start2);
		DrawStopNode(stop);
		DrawStopNode(stop2);
		for (unsigned i = 0; i < nLinks; i++) {
			DrawLink(links[i]);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}