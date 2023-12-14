#include <raylib.h>

#include "codeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"

int main() {
	InitWindow(1020, 800, "Interschem");

	NodeInfo cNode;
	cNode.id = -1;
	cNode.index = -1;
	cNode.type = none;

	NodeArrays nodes;

	NewNode(nodes, start, 5, 20, 500, 100);
	NewNode(nodes, read, 5, 20, 500, 200);
	NewNode(nodes, decision, 5, 20, 500, 300);
	NewNode(nodes, write, 5, 20, 700, 400);
	NewNode(nodes, assign, 5, 20, 300, 400);
	NewNode(nodes, stop, 5, 20, 500, 500);

	NodeLinks links;
	NewLink(links, nodes.startNode->outPin, nodes.readNodes[0]->inPin);
	NewLink(links, nodes.readNodes[0]->outPin, nodes.decisionNodes[0]->inPin);
	NewLink(links, nodes.decisionNodes[0]->outPinTrue, nodes.assignNodes[0]->inPin);
	NewLink(links, nodes.decisionNodes[0]->outPinFalse, nodes.writeNodes[0]->inPin);
	NewLink(links, nodes.assignNodes[0]->outPin, nodes.stopNodes[0]->inPin);
	NewLink(links, nodes.writeNodes[0]->outPin, nodes.stopNodes[0]->inPin);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// update data
		
		int mx = GetMouseX(), my = GetMouseY();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			GetClickedNodeID(cNode, mx, my, nodes);
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (cNode.id != -1) {
				switch (cNode.type) {
				case start: SetStartNodePosition(nodes.startNode, mx, my); break;
				case read: SetReadNodePosition(nodes.readNodes[cNode.index], mx, my); break;
				case write: SetWriteNodePosition(nodes.writeNodes[cNode.index], mx, my); break;
				case assign: SetAssignNodePosition(nodes.assignNodes[cNode.index], mx, my); break;
				case decision: SetDecisionNodePosition(nodes.decisionNodes[cNode.index], mx, my); break;
				case stop: SetStopNodePosition(nodes.stopNodes[cNode.index], mx, my); break;
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

		DrawStartNode(nodes.startNode);
		for (unsigned i = 0; i < nodes.nReadNodes; i++) {
			DrawReadNode(nodes.readNodes[i]);
		}
		for (unsigned i = 0; i < nodes.nWriteNodes; i++) {
			DrawWriteNode(nodes.writeNodes[i]);
		}
		for (unsigned i = 0; i < nodes.nAssignNodes; i++) {
			DrawAssignNode(nodes.assignNodes[i]);
		}
		for (unsigned i = 0; i < nodes.nDecisionNodes; i++) {
			DrawDecisionNode(nodes.decisionNodes[i]);
		}
		for (unsigned i = 0; i < nodes.nStopNodes; i++) {
			DrawStopNode(nodes.stopNodes[i]);
		}
		for (unsigned i = 0; i < links.nLinks; i++) {
			DrawLink(links.vec[i]);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}