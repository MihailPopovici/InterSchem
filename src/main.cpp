#include <raylib.h>

#include "nodeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"
#include "fileFunc.h"
#include "error.h"

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

NodeArrays nodes;

int main() {
	int windowWidth = 1080, windowHeight = 800;
	InitWindow(windowWidth, windowHeight, "Interschem");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetExitKey(0);

	AnyNodeType dragNode{ nullptr, noType };
	AnyNodeType selectedNode{ nullptr, noType };
	Pin* selectedPin = nullptr;

	EditorState editorState = EditorStateNormal;
	ErrorState errState = ErrorState_notErroring;

	Button* deleteSelectedNode = NewButton();
	SetButtonColors(deleteSelectedNode, RED, WHITE);
	SetButtonLabel(deleteSelectedNode, "Delete", 16, 5);

	Button* exec = NewButton();
	SetButtonColors(exec, GREEN, RAYWHITE);
	SetButtonLabel(exec, "Execute", 20, 5);
	SetButtonPosition(exec, windowWidth - exec->width, 0);
	exec->visible = true;

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
	Button* showCodeWindow = NewButton();
	SetButtonColors(showCodeWindow, { 66,66,66,255 }, RAYWHITE);
	SetButtonLabel(showCodeWindow, "Code", 20, 5);
	SetButtonPosition(showCodeWindow, showSchemesWindow->x + showSchemesWindow->width + 5, 0);

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
	AddElementToWindow(createNodesWin, { createStartNode, WindowElementType_Button });
	AddElementToWindow(createNodesWin, { createReadNode, WindowElementType_Button });
	AddElementToWindow(createNodesWin, { createWriteNode, WindowElementType_Button });
	AddElementToWindow(createNodesWin, { createAssignNode, WindowElementType_Button });
	AddElementToWindow(createNodesWin, { createDecisionNode, WindowElementType_Button });
	AddElementToWindow(createNodesWin, { createStopNode, WindowElementType_Button });
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
	AddElementToWindow(variablesWin, { variablesDictionary, WindowElementType_Dictionary });
	variablesWin->visible = false;

	Font font = LoadFont("resources/IBMPlexMono-Medium.ttf");
	MultiLineText* console = NewMultiLineText(650, 500, 8, 32, font, 20.0f, 5.0f, WHITE, BLACK);
	MultiLineTextOverrideLine(console, 0, "");
	Window* consoleWin = NewWindow();
	SetWindowColor(consoleWin, { 66, 66, 66, 255 });
	SetWindowPosition(consoleWin, 700, 530);
	SetWindowSpacing(consoleWin, 5);
	SetWindowPadding(consoleWin, 5);
	SetWindowTitle(consoleWin, "Console", 32, RAYWHITE);
	AddElementToWindow(consoleWin, { console, WindowElementType_MultiLineText });
	Button* clearConsole = NewButton();
	SetButtonColors(clearConsole, BLANK, RAYWHITE);
	SetButtonLabel(clearConsole, "Clear", 20, 5);
	AddElementToWindow(consoleWin, { clearConsole, WindowElementType_Button });
	consoleWin->visible = false;

	Grid* schemes = NewGrid();
	SetGridSize(schemes, 2);
	SetGridColor(schemes, BLANK);
	SetGridPadding(schemes, 5);
	SetGridSpacing(schemes, 5);
	auto schemeFiles = GetSchemeFileNames();
	for (auto& name : schemeFiles) {
		Button* b = NewButton();
		SetButtonColors(b, { 100, 100, 100, 100 }, RAYWHITE);
		SetButtonLabel(b, name, 20, 5);
		GridAddElement(schemes, b);
	}
	Button* newScheme = NewButton();
	SetButtonColors(newScheme, { 150,150,150,150 }, RAYWHITE);
	SetButtonLabel(newScheme, "New scheme", 20, 5);
	Button* refreshSchemes = NewButton();
	SetButtonColors(refreshSchemes, { 150,150,150,150 }, RAYWHITE);
	SetButtonLabel(refreshSchemes, "Refresh", 20, 5);
	Window* schemesWin = NewWindow();
	SetWindowColor(schemesWin, { 66, 66, 66, 255 });
	SetWindowPosition(schemesWin, 500, 500);
	SetWindowSpacing(schemesWin, 5);
	SetWindowPadding(schemesWin, 5);
	SetWindowTitle(schemesWin, "Schemes", 32, RAYWHITE);
	Button* textSaveFile = NewButton();
	SetButtonColors(textSaveFile, BLANK, RAYWHITE);
	SetButtonLabel(textSaveFile, "Current scheme file name:", 20, 5);
	SingleLineText* writeFileName = NewSingleLineText();
	SetSingleLineTextColors(writeFileName, { 50, 50, 50, 255 }, RAYWHITE);
	SetSingleLineTextFontSize(writeFileName, 20);
	SetSingleLineTextPadding(writeFileName, 5);
	Button* saveFile = NewButton();
	SetButtonColors(saveFile, { 150,150,150,150 }, RAYWHITE);
	SetButtonLabel(saveFile, "Save", 20, 5);
	AddElementToWindow(schemesWin, { newScheme, WindowElementType_Button });
	AddElementToWindow(schemesWin, { schemes, WindowElementType_Grid });
	AddElementToWindow(schemesWin, { refreshSchemes, WindowElementType_Button });
	AddElementToWindow(schemesWin, { textSaveFile, WindowElementType_Button });
	AddElementToWindow(schemesWin, { writeFileName, WindowElementType_SingleLineText });
	AddElementToWindow(schemesWin, { saveFile, WindowElementType_Button });
	schemesWin->visible = false;
	string currentFilePath = "";

	MultiLineText* code = NewMultiLineText(650, 500, 25, 32, font, 20.0f, 5.0f, WHITE, BLACK);
	MultiLineTextOverrideLine(code, 0, "");
	Window* codeWin = NewWindow();
	SetWindowColor(codeWin, { 66, 66, 66, 255 });
	SetWindowPosition(codeWin, 650, 50);
	SetWindowSpacing(codeWin, 5.0f);
	SetWindowPadding(codeWin, 5.0f);
	SetWindowTitle(codeWin, "C++ Code", 32, RAYWHITE);
	AddElementToWindow(codeWin, { code, WindowElementType_MultiLineText });
	Button* translate = NewButton();
	SetButtonColors(translate, BLANK, WHITE);
	SetButtonLabel(translate, "Translate", 20, 5);
	AddElementToWindow(codeWin, { translate, WindowElementType_Button });
	codeWin->visible = false;
	
	bool popup = false;
	string popupMsg;

	int dx = 0, dy = 0;
	SetTargetFPS(90);
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

			ClearBackground({ 100, 100, 100, 100 });
			int popupWidth = MeasureText(popupMsg.c_str(), 32);
			DrawText(popupMsg.c_str(), (windowWidth - popupWidth) / 2, (windowHeight - 32) / 2, 32, BLACK);

			EndDrawing();
			windowWidth = GetScreenWidth(), windowHeight = GetScreenHeight();
			continue;
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
		EditSingleLineText(writeFileName);
		UpdateWindow(codeWin);
		showConsoleWindow->visible = WindowShouldClose(consoleWin);
		showCreateWindow->visible = WindowShouldClose(createNodesWin);
		showVariablesWindow->visible = WindowShouldClose(variablesWin);
		showSchemesWindow->visible = WindowShouldClose(schemesWin);
		showCodeWindow->visible = WindowShouldClose(codeWin);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			void* clickedScheme = GetGridClickedElement(schemes);
			if (clickedScheme != nullptr) {
				string filename = ((Button*)clickedScheme)->label;
				string path = "schemes/" + filename + ".interschem";
				CleanupNodes(nodes);
				bool success = LoadSchemeFromFile(nodes, path);
				currentFilePath = path;
				writeFileName->str = filename;
				ResizeSingleLineText(writeFileName);
				dragNode = selectedNode = { nullptr, noType };
				selectedPin = nullptr;
				currentNode = { nodes.startNode, start };
				state = notExecuting;
				ResetDictionary(variablesDictionary);
			}

			GetClickedNode(dragNode, mx, my, nodes);
			if (dragNode.type != noType && dragNode.address != nullptr) {
				int* pos = (int*)dragNode.address;
				dx = *pos, dy = *(pos + 1);
				dx -= mx, dy -= my;
			}

			Pin* secondPin = nullptr;
			GetClickedPin(secondPin, nodes);
			if (selectedPin == nullptr && secondPin != nullptr && secondPin->type == output) {
				selectedPin = secondPin;
				if (selectedPin != nullptr) {
					switch (selectedPin->ownerType) {
					case start: ((StartNode*)selectedPin->ownerPtr)->toPin = nullptr; break;
					case decision: {
						DecisionNode* d = (DecisionNode*)selectedPin->ownerPtr;
						if (selectedPin == &d->outPinTrue) {
							d->toPinTrue = nullptr;
						}
						else {
							d->toPinFalse = nullptr;
						}
						break;
					}
					default:
						*(Pin**)((Pin*)((int*)selectedPin->ownerPtr + 6) + 2) = nullptr;
					}
				}
			}
			else if (selectedPin != nullptr && secondPin != nullptr && secondPin->type == input) {
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
			}
			else {
				selectedPin = nullptr;
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
				deleteSelectedNode->visible = true;
			}
			else {
				deleteSelectedNode->visible = false;
			}
		}

		if (IsButtonClicked(deleteSelectedNode)) {
			EraseNode(nodes, selectedNode);
			selectedNode = { nullptr, noType };
		}

		if (IsButtonClicked(exec)) {
			errState = ErrorState_notErroring;
			CheckForIncompleteScheme(nodes, errState);
			if (errState == ErrorState_incompleteScheme) {
				popup = true;
				popupMsg = "Incomplete Scheme";
				continue;
			}
			errState = ErrorState_notErroring;
			CheckForErrors(nodes, variablesDictionary, errState);
			if (errState == ErrorState_incorrectVarName) {
				popup = true;
				popupMsg = "Incorrect Variable Name";
				continue;
			}
			else if (errState == ErrorState_incorrectMathExpression) {
				popup = true;
				popupMsg = "Incorrect Mathematical Expression";
				continue;
			}
			else if (errState == ErrorState_incorrectLogicalExpression) {
				popup = true;
				popupMsg = "Incorrect Logical Expression";
				continue;
			}
			else if (errState == ErrorState_emptyNode) {
				popup = true;
				popupMsg = "Empty Node";
				continue;
			}
			MultiLineTextClear(console);
			UpdateVariablesTable(nodes, variablesDictionary);
			GetNextNodeInExecution(currentNode, state, variablesDictionary, console);
		}
		if (IsButtonClicked(createStartNode)) {
			currentNode = dragNode = NewNode(nodes, start, 15, 20, mx, my);
		}
		if (IsButtonClicked(createReadNode)) {
			dragNode = NewNode(nodes, read, 20, 20, mx, my);
		}
		if (IsButtonClicked(createWriteNode)) {
			dragNode = NewNode(nodes, write, 20, 20, mx, my);
		}
		if (IsButtonClicked(createAssignNode)) {
			dragNode = NewNode(nodes, assign, 10, 20, mx, my);
		}
		if (IsButtonClicked(createDecisionNode)) {
			dragNode = NewNode(nodes, decision, 25, 20, mx, my);
		}
		if (IsButtonClicked(createStopNode)) {
			dragNode = NewNode(nodes, stop, 15, 20, mx, my);
		}
		if (IsButtonClicked(clearConsole)) {
			MultiLineTextClear(console);
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
		if (IsButtonClicked(showCodeWindow)) {
			WindowSetVisible(codeWin, true);
			showCodeWindow->visible = false;
		}
		if (IsButtonClicked(translate)) {
			errState = ErrorState_notErroring;
			CheckForIncompleteScheme(nodes, errState);
			if (errState == ErrorState_incompleteScheme) {
				popup = true;
				popupMsg = "Incomplete Scheme";
				continue;
			}
			errState = ErrorState_notErroring;
			CheckForErrors(nodes, variablesDictionary, errState);
			if (errState == ErrorState_incorrectVarName) {
				popup = true;
				popupMsg = "Incorrect Variable Name";
				continue;
			}
			else if (errState == ErrorState_incorrectMathExpression) {
				popup = true;
				popupMsg = "Incorrect Mathematical Expression";
				continue;
			}
			else if (errState == ErrorState_incorrectLogicalExpression) {
				popup = true;
				popupMsg = "Incorrect Logical Expression";
				continue;
			}
			else if (errState == ErrorState_emptyNode) {
				popup = true;
				popupMsg = "Empty Node";
				continue;
			}
			MultiLineTextClear(code);
			UpdateVariablesTable(nodes, variablesDictionary);
			EncodeScheme(code, variablesDictionary, nodes.startNode);
		}
		if (IsButtonClicked(refreshSchemes)) {
			GridCleanup(schemes);
			auto schemeFiles = GetSchemeFileNames();
			for (auto& name : schemeFiles) {
				Button* b = NewButton();
				SetButtonColors(b, { 100, 100, 100, 100 }, RAYWHITE);
				SetButtonLabel(b, name, 20, 5);
				GridAddElement(schemes, b);
			}
			ResizeWindow(schemesWin);
			WindowSetVisible(schemesWin, true);
		}
		if (IsButtonClicked(saveFile)) {
			if (currentFilePath == "") {
				if (!writeFileName->str.empty() /*TODO: && ValidFileName(writeFileName->str) && file name doesn't already exist*/) {
					currentFilePath = "schemes/" + writeFileName->str + ".interschem";
					bool success = SaveSchemeToFile(nodes, currentFilePath, false);
					GridCleanup(schemes);
					auto schemeFiles = GetSchemeFileNames();
					for (auto& name : schemeFiles) {
						Button* b = NewButton();
						SetButtonColors(b, { 100, 100, 100, 100 }, RAYWHITE);
						SetButtonLabel(b, name, 20, 5);
						GridAddElement(schemes, b);
					}
					ResizeWindow(schemesWin);
					WindowSetVisible(schemesWin, true);
				}
				else {
					popup = true;
					popupMsg = "Invalid file name";
					continue;
				}
			}
			else {
				bool success = SaveSchemeToFile(nodes, currentFilePath, true);
			}
		}
		if (IsButtonClicked(newScheme)) {
			CleanupNodes(nodes);
			currentFilePath.clear();
			writeFileName->str.clear();
			ResizeSingleLineText(writeFileName);
			dragNode = selectedNode = { nullptr, noType };
			selectedPin = nullptr;
			currentNode = { nodes.startNode, start };
			state = notExecuting;
		}
		
		MultiLineTextEdit(console);
		MultiLineTextEdit(code);
		if (state == notExecuting) {

		}
		else if (state == waitingForInput) {
			cout << "Waiting for input\n";
			if (IsKeyPressed(KEY_ENTER)) {
				float x;
				bool err = MultiLineTextGetNextInt(console, x); // TODO:
				if (!err) {
					ReadNode* p = (ReadNode*)currentNode.address;
					auto drow = GetDictionaryRow(variablesDictionary, p->myVarName->str);
					SetDictionaryRowValue(drow, x);
					GetNextNodeInExecution(currentNode, state, variablesDictionary, console);
				}
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
		DrawButton(showCodeWindow);

		if (selectedPin != nullptr) {
			DrawGhostLink(selectedPin, mx, my);
		}
		if (selectedNode.address != nullptr) {
			DrawSelectedNodeOptions(selectedNode, deleteSelectedNode);
		}

		DrawNodes(nodes);

		DrawButton(exec);

		EndDrawing();
	}

	CleanupNodes(nodes);
	CloseWindow();
	return 0;
}