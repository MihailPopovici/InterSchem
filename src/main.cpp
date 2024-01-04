#include <raylib.h>

#include "nodeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"
#include "fileFunc.h"

#include <iostream>
using namespace std;

#ifndef LOG_LINE(msg)
#define LOG_LINE(msg) MultiLineTextPushLine(console, msg); MultiLineTextPushLine(console, ""); MultiLineTextSetLimitMax(console)
#endif // LOG_LINE(msg)

enum EditorState {
	EditorStateNormal,
	EditorStateSelectedNode,
	EditorStateAddingNode, // TODO:
	EditorStateEditingNode,
	EditorStateAddingLink,
	EditorStateEditingLink, // TODO:
	EditorStateLinkingVariable
};

int main() {
	InitWindow(1080, 800, "Interschem");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	AnyNodeType dragNode{ nullptr, noType };
	AnyNodeType selectedNode{ nullptr, noType };
	Pin* selectedPin = nullptr;

	EditorState edState = EditorStateNormal;

	Button* del = NewButton();
	SetButtonColors(del, RED, WHITE);
	SetButtonLabel(del, "Delete", 16, 5);
	Button* edit = NewButton();
	SetButtonColors(edit, BLUE, WHITE);
	SetButtonLabel(edit, "Edit", 16, 5);
	Button* linkVar = NewButton();
	SetButtonColors(linkVar, PURPLE, WHITE);
	SetButtonLabel(linkVar, "Link", 16, 5);

	Button* exec = NewButton();
	SetButtonColors(exec, GREEN, BLACK);
	SetButtonPosition(exec, 5, 5);
	SetButtonLabel(exec, "Execute", 20, 5);

	NodeArrays nodes;

	ExecutionState state = notExecuting;
	AnyNodeType currentNode{ nullptr, noType };

	Button* createStartNode = NewButton();
	SetButtonColors(createStartNode, { 0, 100, 32, 255 }, WHITE);
	SetButtonLabel(createStartNode, "New Start Node", 20, 5);
	Button* createReadNode = NewButton();
	SetButtonColors(createReadNode, { 210,100,100,255 }, WHITE);
	SetButtonLabel(createReadNode, "New Read Node", 20, 5);
	Button* createWriteNode = NewButton();
	SetButtonColors(createWriteNode, { 0,121,255,255 }, WHITE);
	SetButtonLabel(createWriteNode, "New Write Node", 20, 5);
	Button* createAssignNode = NewButton();
	SetButtonColors(createAssignNode, { 20,200,200,255 }, WHITE);
	SetButtonLabel(createAssignNode, "New Assign Node", 20, 5);
	Button* createDecisionNode = NewButton();
	SetButtonColors(createDecisionNode, { 80,20,200,255 }, WHITE);
	SetButtonLabel(createDecisionNode, "New Decision Node", 20, 5);
	Button* createStopNode = NewButton();
	SetButtonColors(createStopNode, { 155, 40, 60, 255 }, WHITE);
	SetButtonLabel(createStopNode, "New Stop Node", 20, 5);
	Window* createNodes = NewWindow();
	SetWindowColor(createNodes, { 66, 66, 66, 255 });
	SetWindowPosition(createNodes, 5, 40);
	SetWindowSpacing(createNodes, 5.0f);
	SetWindowPadding(createNodes, 5.0f);
	AddElementToWindow(createNodes, { createStartNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createReadNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createWriteNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createAssignNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createDecisionNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createStopNode, WindowElementTypeButton });

	Dictionary* dict = NewDictionary();
	SetDictionaryPosition(dict, 100, 100);
	SetDictionaryPadding(dict, 5);
	SetDictionarySpacing(dict, 5);
	Window* variables = NewWindow();
	SetWindowColor(variables, { 66, 66, 66, 255 });
	SetWindowPosition(variables, 5, 300);
	SetWindowSpacing(variables, 5.0f);
	SetWindowPadding(variables, 5.0f);
	AddElementToWindow(variables, { dict, WindowElementTypeDictionary });
	DictionaryRowHalf rhalf = { DictionaryRowHalfType_None, nullptr };
	DictionaryRow* selectedRow = nullptr;

	Font font = LoadFont("resources/IBMPlexMono-Medium.ttf");
	MultiLineText* console = NewMultiLineText(650, 500, 8, 32, font, 20.0f, 5.0f, WHITE, BLACK);
	MultiLineTextOverrideLine(console, 0, "");
	MultiLineTextSetLimitMax(console);
	Window* consoleWin = NewWindow();
	SetWindowColor(consoleWin, { 66, 66, 66, 255 });
	SetWindowPosition(consoleWin, 650, 550);
	SetWindowSpacing(consoleWin, 5.0f);
	SetWindowPadding(consoleWin, 5.0f);
	AddElementToWindow(consoleWin, { console, WindowElementTypeMultiLineText });
	Button* clearConsole = NewButton();
	SetButtonColors(clearConsole, BLANK, WHITE);
	SetButtonLabel(clearConsole, "Clear", 20, 5);
	AddElementToWindow(consoleWin, { clearConsole, WindowElementTypeButton });

	NodeArrays secondNodes;

	int dx = 0, dy = 0;
	SetTargetFPS(120);
	while (!WindowShouldClose()) {
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
			SaveSchemeToFile(nodes);
			LOG_LINE("Saved");
		}
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
			CleanupNodes(nodes);
			nodes = LoadSchemeFromFile();
			dragNode = selectedNode = { nullptr, noType };
			selectedPin = nullptr;
			currentNode = { nodes.startNode, start };
			edState = EditorStateNormal;
			state = notExecuting;
			LOG_LINE("Loaded");
		}
		MultiLineTextEdit(console);

		for (AssignNode* p : nodes.assignNodes) {
			GetInputAssignNode(p);
		}
		for (DecisionNode* p : nodes.decisionNodes) {
			GetInputDecisionNode(p);
		}
		for (WriteNode* p : nodes.writeNodes) {
			GetInputWriteNode(p);
		}
		for (ReadNode* p : nodes.readNodes) {
			GetInputReadNode(p);
		}

		// update data
		int mx = GetMouseX(), my = GetMouseY();

		DragWindow(consoleWin);
		DragWindow(createNodes);
		DragWindow(variables);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (edState == EditorStateNormal) {
				GetClickedNode(dragNode, mx, my, nodes);
				if (dragNode.type != noType && dragNode.address != nullptr) {
					int* pos = (int*)dragNode.address;
					dx = *pos, dy = *(pos + 1);
					dx -= mx, dy -= my;
				}
			}

			if (edState == EditorStateEditingNode) {
				GetClickedPin(selectedPin, mx, my, nodes);
				if (selectedPin != nullptr && selectedPin->type == output) {
					selectedNode.address = selectedPin->ownerPtr; //TODO: this should not be necessary
					selectedNode.type = selectedPin->ownerType;

					edState = EditorStateAddingLink;
				}
			}
			else if (edState == EditorStateAddingLink) {
				Pin* secondPin = nullptr;
				GetClickedPin(secondPin, mx, my, nodes);
				if (secondPin != nullptr && secondPin->type == input && selectedPin != nullptr) {
					switch (selectedPin->ownerType) {
					case start: NewLink(((StartNode*)selectedPin->ownerPtr)->toPin, *secondPin); break;
					case read: NewLink(((ReadNode*)selectedPin->ownerPtr)->toPin, *secondPin); break;
					case write: NewLink(((WriteNode*)selectedPin->ownerPtr)->toPin, *secondPin); break;
					case assign: NewLink(((AssignNode*)selectedPin->ownerPtr)->toPin, *secondPin); break;
					case decision:
						if (selectedPin == &((DecisionNode*)selectedPin->ownerPtr)->outPinTrue)
							NewLink(((DecisionNode*)selectedPin->ownerPtr)->toPinTrue, *secondPin);
						else
							NewLink(((DecisionNode*)selectedPin->ownerPtr)->toPinFalse, *secondPin);
						break;
					default: break;
					}
					selectedPin = nullptr;
					edState = EditorStateEditingNode;
				}
			}
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			DragNode(dragNode, dx, dy);
		}
		else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			dragNode = { nullptr, noType }; // TODO: performance?
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			GetClickedNode(selectedNode, mx, my, nodes);
			if (selectedNode.address != nullptr) {
				edState = EditorStateSelectedNode;
			}
			else {
				edState = EditorStateNormal;
			}
		}

		if (selectedNode.address != nullptr) {
			if (IsButtonClicked(edit)) {
				edState = EditorStateEditingNode;
			}
			if (IsButtonClicked(del)) {
				EraseNode(nodes, selectedNode);
				selectedNode = { nullptr, noType };
				edState = EditorStateNormal;
			}
			if (IsButtonClicked(linkVar)) {
				edState = EditorStateLinkingVariable;
			}
		}

		if (IsButtonClicked(exec)) {
			UpdateVariablesTable(nodes, dict);
			GetNextNodeInExecution(currentNode, state, dict, console);
		}
		if (IsButtonClicked(createStartNode)) {
			currentNode = dragNode = NewNode(nodes, start, 5, 20, mx, my);
		}
		if (IsButtonClicked(createReadNode)) {
			dragNode = NewNode(nodes, read, 5, 20, mx, my);
		}
		if (IsButtonClicked(createWriteNode)) {
			dragNode = NewNode(nodes, write, 5, 20, mx, my);
		}
		if (IsButtonClicked(createAssignNode)) {
			dragNode = NewNode(nodes, assign, 5, 20, mx, my);
		}
		if (IsButtonClicked(createDecisionNode)) {
			dragNode = NewNode(nodes, decision, 5, 20, mx, my);
		}
		if (IsButtonClicked(createStopNode)) {
			dragNode = NewNode(nodes, stop, 5, 20, mx, my);
		}
		if (IsButtonClicked(clearConsole)) {
			MultiLineTextClear(console);
		}

		if (state == notExecuting) {

		}
		else if (state == waitingForInput) {
			cout << "Waiting for input\n";
			if (IsKeyPressed(KEY_ENTER)) {
				int x = MultiLineTextGetNextInt(console);
				MultiLineTextSetLimitMax(console);
				ReadNode* p = (ReadNode*)currentNode.address;
				auto drow = GetDictionaryRow(dict, p->myVarName->str);
				SetDictionaryRowValue(drow, x);
				GetNextNodeInExecution(currentNode, state, dict, console);
			}
		}
		else if (state == processing) {
			cout << "Processing\n";
			GetNextNodeInExecution(currentNode, state, dict, console);
		}
		else {
			cout << "Done\n";
			state = notExecuting;
			currentNode = { nodes.startNode, start };
		}

		BeginDrawing();
		// render on screen
		ClearBackground({ 33, 33, 33 });

		DrawWindow(consoleWin);
		DrawWindow(createNodes);
		DrawWindow(variables);

		if (edState == EditorStateAddingLink) {
			DrawGhostLink(selectedPin, mx, my);
		}
		if (edState != EditorStateNormal) {
			DrawSelectedNodeOptions(selectedNode, del, edit, linkVar);
		}

		DrawNodes(nodes);

		DrawButton(exec);

		EndDrawing();
	}

	CleanupNodes(nodes);
	CloseWindow();
	return 0;
}