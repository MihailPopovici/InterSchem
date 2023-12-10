#include <raylib.h>

#include "codeComponents.h"
#include "uiComponents.h"


int main() {
	InitWindow(1020, 800, "Interschem");

	unsigned globalPinID = 0, globalNodeID = 0;

	StartNode* start = NewStartNode(globalPinID);
	SetStartNodePosition(start, 100, 100);
	SetStartNodeSize(start, 5, 20);

	ReadNode* clickedReadNode = nullptr;
	int clickedNodeID = -1;
	ReadNode* read = NewReadNode(globalPinID);
	SetReadNodePosition(read, 200, 200);
	SetReadNodeSize(read, 5, 20);

	WriteNode* write = NewWriteNode(globalPinID);
	SetWriteNodePosition(write, 400, 400);
	SetWriteNodeSize(write, 5, 20);

	AssignNode* assign = NewAssignNode(globalPinID);
	SetAssignNodePosition(assign, 600, 600);
	SetAssignNodeSize(assign, 5, 20);

	StopNode* stop = NewStopNode(globalPinID);
	SetStopNodePosition(stop, 700, 700);
	SetStopNodeSize(stop, 5, 20);

	unsigned nLinks = 0;
	Link links[16];
	links[0] = NewLink(start, read, nLinks);
	links[1] = NewLink(read, write, nLinks);
	links[2] = NewLink(write, assign, nLinks);
	links[3] = NewLink(assign, stop, nLinks);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// update data
		Vector2 mPos = GetMousePosition();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if ((mPos.x >= read->x && mPos.x <= read->x + read->width) && (mPos.y >= read->y && mPos.y <= read->y + read->height)) {
				clickedReadNode = read;
			}
			else {
				clickedReadNode = nullptr;
			}
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (clickedReadNode != nullptr) {
				SetReadNodePosition(clickedReadNode, mPos.x, mPos.y);
			}
		}
		else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			clickedReadNode = nullptr;
		}

		BeginDrawing();
		ClearBackground(BLACK);
		// render on screen

		DrawStartNode(start);
		DrawReadNode(read);
		DrawWriteNode(write);
		DrawAssignNode(assign);
		DrawStopNode(stop);
		for (unsigned i = 0; i < nLinks; i++) {
			DrawLink(links[i]);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}