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

	Button* exec = NewButton();
	SetButtonColors(exec, GREEN, BLACK);
	SetButtonPosition(exec, 5, 5);
	SetButtonLabel(exec, "Execute", 20, 5);
	string input = "";
	string output = "";

	NodeArrays nodes;

	NewNode(nodes, start, 5, 20, 500, 100);
	NewNode(nodes, read, 5, 20, 500, 200);
	//NewNode(nodes, decision, 5, 20, 500, 300);
	//NewNode(nodes, assign, 5, 20, 300, 400);
	NewNode(nodes, write, 5, 20, 700, 400);
	NewNode(nodes, stop, 5, 20, 500, 500);

	ExecutionState state = notExecuting;
	void* currentNode = nodes.startNode;
	NodeType currentNodeType = start;

	NewLink(nodes.startNode->toPin, nodes.readNodes[0]->inPin);
	NewLink(nodes.readNodes[0]->toPin, nodes.writeNodes[0]->inPin);
	NewLink(nodes.writeNodes[0]->toPin, nodes.stopNodes[0]->inPin);

	int x = 2;
	nodes.readNodes[0]->myVar = &x;
	nodes.writeNodes[0]->myVar = &x;

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

		if (IsButtonClicked(exec)) {
			GetNextNodeInExecution(currentNode, currentNodeType, state);
		}

		if (state == notExecuting) {

		}
		else if (state == waitingForInput) {
			cout << "Waiting for input\n";
			char c = GetCharPressed();
			if (c != 0 && (c >= '0' && c <= '9')) {
				input.insert(input.end(), c);
			}

			if (IsKeyPressed(KEY_ENTER)) {
				x = stoi(input);
				SetValue((ReadNode*)currentNode, x);
				GetNextNodeInExecution(currentNode, currentNodeType, state);
			}
		}
		else if (state == processing) {
			cout << "Processing\n";
			if (currentNodeType == write) {
				WriteValue((WriteNode*)currentNode);
			}
			GetNextNodeInExecution(currentNode, currentNodeType, state);
		}
		else {
			cout << "Done\n";
			state = notExecuting;
			output = "Result: " + to_string(*nodes.writeNodes[0]->myVar);
		}

		BeginDrawing();
		ClearBackground(BLACK);
		// render on screen

		DrawStartNode(nodes.startNode);
		for (StopNode* p : nodes.stopNodes) {
			DrawStopNode(p);
		}
		for (ReadNode* p : nodes.readNodes) {
			DrawReadNode(p);
		}
		for (WriteNode* p : nodes.writeNodes) {
			DrawWriteNode(p);
		}
		for (AssignNode* p : nodes.assignNodes) {
			DrawAssignNode(p);
		}
		for (DecisionNode* p : nodes.decisionNodes) {
			DrawDecisionNode(p);
		}
		DrawButton(exec);

		if (state == waitingForInput) {
			DrawText(input.c_str(), 5, 35, 20, WHITE);
		}
		else if (!output.empty()) {
			DrawText(output.c_str(), 5, 35, 20, WHITE);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}