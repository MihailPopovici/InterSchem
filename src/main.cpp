#include <raylib.h>

#include "codeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"

int main() {
	InitWindow(1020, 800, "Interschem");

	int globalPinID = 0, globalNodeID = 0;
	ClickedNodeInfo cNode;
	cNode.id = -1;
	cNode.index = -1;
	cNode.type = none;

	StartNode* startNode = NewStartNode(globalNodeID, globalPinID);
	SetStartNodePosition(startNode, 100, 100);
	SetStartNodeSize(startNode, 5, 20);

	unsigned nReadNodes = 0;
	ReadNode* readNodes[16] = { 0 };
	readNodes[0] = NewReadNode(globalNodeID, globalPinID);
	SetReadNodePosition(readNodes[0], 200, 200);
	SetReadNodeSize(readNodes[0], 5, 20);
	nReadNodes++;

	unsigned nWriteNodes = 0;
	WriteNode* writeNodes[16] = { 0 };
	writeNodes[0] = NewWriteNode(globalNodeID, globalPinID);
	SetWriteNodePosition(writeNodes[0], 400, 400);
	SetWriteNodeSize(writeNodes[0], 5, 20);
	nWriteNodes++;

	unsigned nAssignNodes = 0;
	AssignNode* assignNodes[16] = { 0 };
	assignNodes[0] = NewAssignNode(globalNodeID, globalPinID);
	SetAssignNodePosition(assignNodes[0], 600, 600);
	SetAssignNodeSize(assignNodes[0], 5, 20);
	nAssignNodes++;

	unsigned nStopNodes = 0;
	StopNode* stopNodes[16] = { 0 };
	stopNodes[0] = NewStopNode(globalNodeID, globalPinID);
	SetStopNodePosition(stopNodes[0], 700, 700);
	SetStopNodeSize(stopNodes[0], 5, 20);
	nStopNodes++;

	unsigned nLinks = 0;
	Link links[16];
	links[0] = NewLink(startNode, readNodes[0], nLinks);
	links[1] = NewLink(readNodes[0], writeNodes[0], nLinks);
	links[2] = NewLink(writeNodes[0], assignNodes[0], nLinks);
	links[3] = NewLink(assignNodes[0], stopNodes[0], nLinks);
	links[4] = NewLink(readNodes[0], stopNodes[0], nLinks);
	//links[4] = NewLink(readNodes[0], decisionNodes[0], nLinks);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// update data
		
		int mx = GetMouseX(), my = GetMouseY();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			GetClickedNodeID(cNode, mx, my, startNode, readNodes, writeNodes, assignNodes, stopNodes);
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (cNode.id != -1) {
				switch (cNode.type) {
				case start: { SetStartNodePosition(startNode, mx, my); break; }
				case read: { SetReadNodePosition(readNodes[cNode.index], mx, my); break; }
				case write: { SetWriteNodePosition(writeNodes[cNode.index], mx, my); break; }
				case assign: { SetAssignNodePosition(assignNodes[cNode.index], mx, my); break; }
				//case decision: { SetDecisionNodePosition(decisionNodes[cNode.index], mx, my); break; }
				case stop: { SetStopNodePosition(stopNodes[cNode.index], mx, my); break; }
				default: break;
				}
			}
		}
		else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			cNode.id = -1;
		}

		BeginDrawing();
		ClearBackground(BLACK);
		// render on screen

		DrawStartNode(startNode);
		for (unsigned i = 0; i < nReadNodes; i++) {
			DrawReadNode(readNodes[i]);
		}
		for (unsigned i = 0; i < nWriteNodes; i++) {
			DrawWriteNode(writeNodes[i]);
		}
		for (unsigned i = 0; i < nAssignNodes; i++) {
			DrawAssignNode(assignNodes[i]);
		}
		for (unsigned i = 0; i < nStopNodes; i++) {
			DrawStopNode(stopNodes[i]);
		}
		for (unsigned i = 0; i < nLinks; i++) {
			DrawLink(links[i]);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}