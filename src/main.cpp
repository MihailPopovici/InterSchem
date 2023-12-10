#include <raylib.h>

#include "codeComponents.h"
#include "uiComponents.h"

int main() {
	InitWindow(1020, 800, "Interschem");

	unsigned globalID = 0;

	StartNode* start = NewStartNode(globalID);
	SetStartNodePosition(start, 100, 100);
	SetStartNodeSize(start, 5, 20);

	StopNode* stop = NewStopNode(globalID);
	SetStopNodePosition(stop, 300, 300);
	SetStopNodeSize(stop, 5, 20);

	ReadNode* read = NewReadNode(globalID);
	SetReadNodePosition(read, 150, 150);
	SetReadNodeSize(read, 5, 20);

	WriteNode* write = NewWriteNode(globalID);
	SetWriteNodePosition(write, 200, 200);
	SetWriteNodeSize(write, 5, 20);

	unsigned nLinks = 0;
	Link links[16];
	links[0] = NewLink(start, read, nLinks);
	links[1] = NewLink(read, write, nLinks);
	links[2] = NewLink(write, stop, nLinks);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// update data

		BeginDrawing();
		ClearBackground(BLACK);
		// render on screen

		DrawStartNode(start);
		DrawReadNode(read);
		DrawWriteNode(write);
		DrawStopNode(stop);
		for (unsigned i = 0; i < nLinks; i++) {
			DrawLink(links[i]);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}