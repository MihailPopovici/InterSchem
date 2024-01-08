#include <raylib.h>

#include "nodeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"
#include "fileFunc.h"

#include <iostream>
using namespace std;

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
	int windowWidth = 1080, windowHeight = 800;
	InitWindow(windowWidth, windowHeight, "Interschem");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	AnyNodeType dragNode{ nullptr, noType };
	AnyNodeType selectedNode{ nullptr, noType };
	Pin* selectedPin = nullptr;

	EditorState editorState = EditorStateNormal;

	Button* deleteSelectedNode = NewButton();
	SetButtonColors(deleteSelectedNode, RED, WHITE);
	SetButtonLabel(deleteSelectedNode, "Delete", 16, 5);
	Button* editSelectedNode = NewButton();
	SetButtonColors(editSelectedNode, BLUE, WHITE);
	SetButtonLabel(editSelectedNode, "Edit", 16, 5);

	Button* exec = NewButton();
	SetButtonColors(exec, GREEN, RAYWHITE);
	SetButtonLabel(exec, "Execute", 20, 5);
	SetButtonPosition(exec, windowWidth - exec->width, 0);
	exec->visible = true;

	NodeArrays nodes;

	ExecutionState state = notExecuting;
	AnyNodeType currentNode{ nullptr, noType };

	Button* showCreateWindow = NewButton();
	SetButtonColors(showCreateWindow, { 66,66,66,255 }, RAYWHITE);
	SetButtonLabel(showCreateWindow, "New Nodes", 20, 5);
	SetButtonPosition(showCreateWindow, 0, 0);
	Button* showConsoleWindow = NewButton();
	SetButtonColors(showConsoleWindow, { 66,66,66,255 }, RAYWHITE);
	SetButtonLabel(showConsoleWindow, "Console", 20, 5);
	SetButtonPosition(showConsoleWindow, showCreateWindow->width + 5, 0);
	Button* showVariablesWindow = NewButton();
	SetButtonColors(showVariablesWindow, { 66,66,66,255 }, RAYWHITE);
	SetButtonLabel(showVariablesWindow, "Variables", 20, 5);
	SetButtonPosition(showVariablesWindow, showConsoleWindow->x + showConsoleWindow->width + 5, 0);
	Button* showSchemesWindow = NewButton();
	SetButtonColors(showSchemesWindow, { 66,66,66,255 }, RAYWHITE);
	SetButtonLabel(showSchemesWindow, "Schemes", 20, 5);
	SetButtonPosition(showSchemesWindow, showVariablesWindow->x + showVariablesWindow->width + 5, 0);

	Button* createStartNode = NewButton();
	SetButtonColors(createStartNode, DARKGREEN, WHITE);
	SetButtonLabel(createStartNode, "New Start Node", 20, 5);
	Button* createReadNode = NewButton();
	SetButtonColors(createReadNode, YELLOW, WHITE);
	SetButtonLabel(createReadNode, "New Read Node", 20, 5);
	Button* createWriteNode = NewButton();
	SetButtonColors(createWriteNode, SKYBLUE, WHITE);
	SetButtonLabel(createWriteNode, "New Write Node", 20, 5);
	Button* createAssignNode = NewButton();
	SetButtonColors(createAssignNode, ORANGE, WHITE);
	SetButtonLabel(createAssignNode, "New Assign Node", 20, 5);
	Button* createDecisionNode = NewButton();
	SetButtonColors(createDecisionNode, PURPLE, WHITE);
	SetButtonLabel(createDecisionNode, "New Decision Node", 20, 5);
	Button* createStopNode = NewButton();
	SetButtonColors(createStopNode, RED, WHITE);
	SetButtonLabel(createStopNode, "New Stop Node", 20, 5);
	Window* createNodesWin = NewWindow();
	SetWindowColor(createNodesWin, { 66, 66, 66, 255 });
	SetWindowPosition(createNodesWin, 5, 100);
	SetWindowSpacing(createNodesWin, 5);
	SetWindowPadding(createNodesWin, 5);
	SetWindowTitle(createNodesWin, "New nodes", 32, WHITE);
	AddElementToWindow(createNodesWin, { createStartNode, WindowElementTypeButton });
	AddElementToWindow(createNodesWin, { createReadNode, WindowElementTypeButton });
	AddElementToWindow(createNodesWin, { createWriteNode, WindowElementTypeButton });
	AddElementToWindow(createNodesWin, { createAssignNode, WindowElementTypeButton });
	AddElementToWindow(createNodesWin, { createDecisionNode, WindowElementTypeButton });
	AddElementToWindow(createNodesWin, { createStopNode, WindowElementTypeButton });
	createNodesWin->visible = false;

	Dictionary* variablesDictionary = NewDictionary();
	SetDictionaryPosition(variablesDictionary, 100, 100);
	SetDictionaryPadding(variablesDictionary, 5);
	SetDictionarySpacing(variablesDictionary, 5);
	SetDictionaryColor(variablesDictionary, BLANK);
	Window* variablesWin = NewWindow();
	SetWindowColor(variablesWin, { 66, 66, 66, 255 });
	SetWindowPosition(variablesWin, 5, 400);
	SetWindowSpacing(variablesWin, 5);
	SetWindowPadding(variablesWin, 5);
	SetWindowTitle(variablesWin, "Variables", 32, RAYWHITE);
	AddElementToWindow(variablesWin, { variablesDictionary, WindowElementTypeDictionary });
	variablesWin->visible = false;

	DictionaryRowHalf rhalf = { DictionaryRowHalfType_None, nullptr };
	DictionaryRow* selectedRow = nullptr;

	Font font = LoadFont("resources/IBMPlexMono-Medium.ttf");
	MultiLineText* console = NewMultiLineText(650, 500, 8, 32, font, 20.0f, 5.0f, WHITE, BLACK);
	MultiLineTextOverrideLine(console, 0, "");
	MultiLineTextSetLimitMax(console);
	Window* consoleWin = NewWindow();
	SetWindowColor(consoleWin, { 66, 66, 66, 255 });
	SetWindowPosition(consoleWin, 700, 530);
	SetWindowSpacing(consoleWin, 5);
	SetWindowPadding(consoleWin, 5);
	SetWindowTitle(consoleWin, "Console", 32, RAYWHITE);
	AddElementToWindow(consoleWin, { console, WindowElementTypeMultiLineText });
	Button* clearConsole = NewButton();
	SetButtonColors(clearConsole, BLANK, RAYWHITE);
	SetButtonLabel(clearConsole, "Clear", 20, 5);
	AddElementToWindow(consoleWin, { clearConsole, WindowElementTypeButton });
	consoleWin->visible = false;

	NodeArrays secondNodes;
	auto schemes = GetSchemeFileNames();
	Dictionary* schemeFiles = NewDictionary();
	SetDictionaryPosition(schemeFiles, 100, 100);
	SetDictionaryPadding(schemeFiles, 0);
	SetDictionarySpacing(schemeFiles, 5);
	SetDictionaryColor(schemeFiles, BLANK);
	Window* schemesWin = NewWindow();
	SetWindowColor(schemesWin, { 66, 66, 66, 255 });
	SetWindowPosition(schemesWin, 5, 500);
	SetWindowSpacing(schemesWin, 5);
	SetWindowPadding(schemesWin, 5);
	SetWindowTitle(schemesWin, "Schemes", 32, RAYWHITE);
	AddElementToWindow(schemesWin, { schemeFiles, WindowElementTypeDictionary });
	schemesWin->visible = false;
	for (auto& file : schemes) {
		DictionaryRow* schRow = NewDictionaryRow();
		SetDictionaryRowData(schRow, file, 0, 20, 5);
		SetDictionaryRowColors(schRow, { 55, 55, 55, 200 }, BLACK);
		AddDictionaryRow(schemeFiles, schRow);
	}
	ResizeDictionary(schemeFiles);

	MultiLineText* code = NewMultiLineText(650, 500, 25, 32, font, 20.0f, 5.0f, WHITE, BLACK);
	MultiLineTextOverrideLine(code, 0, "");
	MultiLineTextSetLimitMax(code);
	Window* codeWin = NewWindow();
	SetWindowColor(codeWin, { 66, 66, 66, 255 });
	SetWindowPosition(codeWin, 650, 50);
	SetWindowSpacing(codeWin, 5.0f);
	SetWindowPadding(codeWin, 5.0f);
	SetWindowTitle(codeWin, "Translation", 32, RAYWHITE);
	AddElementToWindow(codeWin, { code, WindowElementTypeMultiLineText });
	Button* translate = NewButton();
	SetButtonColors(translate, BLANK, WHITE);
	SetButtonLabel(translate, "Translate", 20, 5);
	AddElementToWindow(codeWin, { translate, WindowElementTypeButton });
	WindowSetVisible(codeWin, true);
	
	bool popup = false;
	string popupMsg;

	int dx = 0, dy = 0;
	SetTargetFPS(120);
	while (!WindowShouldClose()) {
		if (IsWindowResized()) {
			int nwidth = GetScreenWidth(), nheight = GetScreenHeight();
			float ratioX, ratioY;
			ratioX = (float)windowWidth / consoleWin->x;
			ratioY = (float)windowHeight / consoleWin->y;
			SetWindowPosition(consoleWin, (int)(nwidth / ratioX), (int)(nheight / ratioY));
			// TODO: get rid of this above?
			SetButtonPosition(exec, nwidth - exec->width, 0);
		}

		if (popup) {
			if (GetKeyPressed() != 0) {
				popup = false;
			}

			BeginDrawing();

			ClearBackground({ 200, 200, 200, 100 });
			int popupWidth = MeasureText(popupMsg.c_str(), 32);
			DrawText(popupMsg.c_str(), (windowWidth - popupWidth) / 2, (windowHeight - 32) / 2, 32, BLACK);

			EndDrawing();
			windowWidth = GetScreenWidth(), windowHeight = GetScreenHeight();
			continue;
		}

		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
			bool success = SaveSchemeToFile(nodes, "data1.txt");
			MultiLineTextPushString(console, "Saved\n");
			MultiLineTextSetLimitMax(console);
		}
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
			CleanupNodes(nodes);
			bool success = LoadSchemeFromFile(nodes, "data1.txt");
			dragNode = selectedNode = { nullptr, noType };
			selectedPin = nullptr;
			currentNode = { nodes.startNode, start };
			editorState = EditorStateNormal;
			state = notExecuting;
			MultiLineTextPushString(console, "Loaded\n");
			MultiLineTextSetLimitMax(console);
		}

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

		UpdateWindow(consoleWin);
		UpdateWindow(createNodesWin);
		UpdateWindow(variablesWin);
		UpdateWindow(schemesWin);
		UpdateWindow(codeWin);
		showConsoleWindow->visible = WindowShouldClose(consoleWin);
		showCreateWindow->visible = WindowShouldClose(createNodesWin);
		showVariablesWindow->visible = WindowShouldClose(variablesWin);
		showSchemesWindow->visible = WindowShouldClose(schemesWin);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (editorState == EditorStateNormal) {
				GetClickedNode(dragNode, mx, my, nodes);
				if (dragNode.type != noType && dragNode.address != nullptr) {
					int* pos = (int*)dragNode.address;
					dx = *pos, dy = *(pos + 1);
					dx -= mx, dy -= my;
				}
				auto drow = GetClickedDictionaryRow(schemeFiles);
				if (drow != nullptr) {
					// TODO: save/load file from a list
				}
			}

			if (editorState == EditorStateEditingNode) {
				GetClickedPin(selectedPin, nodes);
				if (selectedPin != nullptr && selectedPin->type == output) {
					selectedNode.address = selectedPin->ownerPtr; //TODO: this should not be necessary
					selectedNode.type = selectedPin->ownerType;

					editorState = EditorStateAddingLink;
				}
			}
			else if (editorState == EditorStateAddingLink) {
				Pin* secondPin = nullptr;
				GetClickedPin(secondPin, nodes);
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
					editorState = EditorStateEditingNode;
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
				editorState = EditorStateSelectedNode;
				deleteSelectedNode->visible = true;
				editSelectedNode->visible = true;
			}
			else {
				editorState = EditorStateNormal;
				deleteSelectedNode->visible = false;
				editSelectedNode->visible = false;
			}
		}

		if (selectedNode.address != nullptr) {
			if (IsButtonClicked(editSelectedNode)) {
				editorState = EditorStateEditingNode;
			}
			if (IsButtonClicked(deleteSelectedNode)) {
				EraseNode(nodes, selectedNode);
				selectedNode = { nullptr, noType };
				editorState = EditorStateNormal;
			}
		}

		if (IsButtonClicked(exec)) {
			MultiLineTextClear(console);
			UpdateVariablesTable(nodes, variablesDictionary);
			GetNextNodeInExecution(currentNode, state, variablesDictionary, console);
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
			MultiLineTextSetLimitMax(console);
		}
		if (IsButtonClicked(showCreateWindow)) {
			WindowSetVisible(createNodesWin, true);
			showCreateWindow->visible = false;
		}
		if (IsButtonClicked(showConsoleWindow)) {
			WindowSetVisible(consoleWin, true);
			showConsoleWindow->visible = false;
		}
		if (IsButtonClicked(showVariablesWindow)) {
			WindowSetVisible(variablesWin, true);
			showVariablesWindow->visible = false;
		}
		if (IsButtonClicked(showSchemesWindow)) {
			WindowSetVisible(schemesWin, true);
			showSchemesWindow->visible = false;
		}
		if (IsButtonClicked(translate)) {
			MultiLineTextClear(code);
			UpdateVariablesTable(nodes, variablesDictionary);
			EncodeScheme(code, variablesDictionary, nodes.startNode);
		}

		MultiLineTextEdit(console);
		MultiLineTextEdit(code);
		if (state == notExecuting) {

		}
		else if (state == waitingForInput) {
			cout << "Waiting for input\n";
			if (IsKeyPressed(KEY_ENTER)) {
				int x = MultiLineTextGetNextInt(console); // TODO:
				MultiLineTextSetLimitMax(console);
				ReadNode* p = (ReadNode*)currentNode.address;
				auto drow = GetDictionaryRow(variablesDictionary, p->myVarName->str);
				SetDictionaryRowValue(drow, x);
				GetNextNodeInExecution(currentNode, state, variablesDictionary, console);
			}
		}
		else if (state == processing) {
			cout << "Processing\n";
			/*if (currentNode.type == read) {
				MultiLineTextPushString(console, "Input value:");
				MultiLineTextSetLimitMax(console);
			}
			else */if (currentNode.type == assign) {
				EvaluateAssignNode((AssignNode*)currentNode.address, variablesDictionary);
			}
			else if (currentNode.type == write) {
				string writeNodeString = to_string(EvaluateWriteNode((WriteNode*)currentNode.address, variablesDictionary));
				MultiLineTextPushString(console, writeNodeString);
				MultiLineTextSetLimitMax(console);
			}
			GetNextNodeInExecution(currentNode, state, variablesDictionary, console);
		}
		else {
			cout << "Done\n";
			MultiLineTextPushString(console, "Program exited successfully");
			MultiLineTextSetLimitMax(console);
			state = notExecuting;
			currentNode = { nodes.startNode, start };
		}

		windowWidth = GetScreenWidth(), windowHeight = GetScreenHeight();
		BeginDrawing();
		// render on screen
		ClearBackground({ 33, 33, 33 });

		DrawWindow(consoleWin);
		DrawWindow(createNodesWin);
		DrawWindow(variablesWin);
		DrawWindow(schemesWin);
		DrawWindow(codeWin);

		DrawButton(showCreateWindow);
		DrawButton(showConsoleWindow);
		DrawButton(showVariablesWindow);
		DrawButton(showSchemesWindow);

		if (editorState == EditorStateAddingLink) {
			DrawGhostLink(selectedPin, mx, my);
		}
		if (editorState != EditorStateNormal) {
			DrawSelectedNodeOptions(selectedNode, deleteSelectedNode, editSelectedNode);
		}

		DrawNodes(nodes);

		DrawButton(exec);
		
		EndDrawing();
	}

	CleanupNodes(nodes);
	CloseWindow();
	return 0;
}