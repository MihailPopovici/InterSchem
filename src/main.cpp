#include <raylib.h>

#include "nodeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"

#include <iostream>
using namespace std;

int main() {
	InitWindow(1020, 800, "Interschem");

	NodeInfo cNode;
	cNode.id = -1;
	cNode.index = -1;
	cNode.type = start;

	NodeArrays nodes;

	NewNode(nodes, start, 5, 20, 500, 100);
	NewNode(nodes, read, 5, 20, 500, 200);
	//NewNode(nodes, decision, 5, 20, 500, 300);
	//NewNode(nodes, assign, 5, 20, 300, 400);
	NewNode(nodes, write, 5, 20, 700, 400);
	NewNode(nodes, stop, 5, 20, 500, 500);

	/*void* ptr = nullptr;
	ptr = nodes.readNodes[0];
	cout << ((ReadNode*)ptr)->id;*/ // TODO: 

	NewLink(nodes.startNode->toPin, nodes.readNodes[0]->inPin);
	NewLink(nodes.readNodes[0]->toPin, nodes.writeNodes[0]->inPin);
	NewLink(nodes.writeNodes[0]->toPin, nodes.stopNodes[0]->inPin);

	int x = 2;
	nodes.readNodes[0]->myVar = &x;
	nodes.writeNodes[0]->myVar = &x;
	Execute(nodes);

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
		for (auto p : nodes.stopNodes) {
			DrawStopNode(p);
		}
		for (auto p : nodes.readNodes) {
			DrawReadNode(p);
		}
		for (auto p : nodes.writeNodes) {
			DrawWriteNode(p);
		}
		for (auto p : nodes.assignNodes) {
			DrawAssignNode(p);
		}
		for (auto p : nodes.decisionNodes) {
			DrawDecisionNode(p);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}