#include <raylib.h>

#include "codeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"


int main() {
	InitWindow(1020, 800, "Interschem");

	int globalPinID = 0, globalNodeID = 0;
	int clickedNodeID = -1;

	StartNode* start = NewStartNode(globalNodeID, globalPinID);
	SetStartNodePosition(start, 100, 100);
	SetStartNodeSize(start, 5, 20);

	unsigned nReadNodes = 0;
	ReadNode* readNodes[16]={0};
	readNodes[0] = NewReadNode(globalNodeID, globalPinID);
	SetReadNodePosition(readNodes[0], 200, 200);
	SetReadNodeSize(readNodes[0], 5, 20);
	nReadNodes++;
	readNodes[1] = NewReadNode(globalNodeID, globalPinID);
	SetReadNodePosition(readNodes[1], 300, 200);
	SetReadNodeSize(readNodes[1], 5, 20);
	nReadNodes++;
	readNodes[2] = NewReadNode(globalNodeID, globalPinID);
	SetReadNodePosition(readNodes[2], 400, 200);
	SetReadNodeSize(readNodes[2], 5, 20);
	nReadNodes++;

	WriteNode* write = NewWriteNode(globalNodeID, globalPinID);
	SetWriteNodePosition(write, 400, 400);
	SetWriteNodeSize(write, 5, 20);

	AssignNode* assign = NewAssignNode(globalNodeID, globalPinID);
	SetAssignNodePosition(assign, 600, 600);
	SetAssignNodeSize(assign, 5, 20);

	StopNode* stop = NewStopNode(globalNodeID, globalPinID);
	SetStopNodePosition(stop, 700, 700);
	SetStopNodeSize(stop, 5, 20);

	unsigned nLinks = 0;
	Link links[16];
	links[0] = NewLink(start, readNodes[0], nLinks);
	links[1] = NewLink(readNodes[0], write, nLinks);
	links[2] = NewLink(write, assign, nLinks);
	links[3] = NewLink(assign, stop, nLinks);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// update data
		Vector2 mPos = GetMousePosition();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			clickedNodeID = GetClickedNodeID(mPos.x, mPos.y, clickedNodeID, readNodes);
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (clickedNodeID != -1) {
				SetReadNodePosition(readNodes[clickedNodeID], mPos.x, mPos.y);
			}
		}
		else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			clickedNodeID = -1;
		}

		BeginDrawing();
		ClearBackground(BLACK);
		// render on screen

		DrawStartNode(start);
		for (unsigned i = 0; i < nReadNodes; i++) {
			DrawReadNode(readNodes[i]);
		}
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