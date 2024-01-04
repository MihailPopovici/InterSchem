#include "readNode.h"

#include "raylib.h"
#include <cstring>
#include <iostream>

ReadNode* NewReadNode(int padding, int fontSize, int x, int y) {
	ReadNode* p = new ReadNode;
	p->fontSize = 0;
	p->padding = 0;

	p->myVarName = NewSingleLineText();
	SetSingleLineTextColors(p->myVarName, { 100, 100, 100, 70 }, WHITE);
	SetSingleLineTextPadding(p->myVarName, 5);
	SetSingleLineTextFontSize(p->myVarName, 20);
	SetSingleLineTextPosition(p->myVarName, 0, 0);

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.type = input;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->inPin.ownerPtr = p;
	p->inPin.ownerType = read;

	p->outPin.type = output;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;
	p->outPin.ownerPtr = p;
	p->outPin.ownerType = read;

	p->toPin = nullptr;

	SetReadNodePosition(p, x, y);
	SetReadNodeSize(p, padding, fontSize);

	return p;
}
void SetReadNodeSize(ReadNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->myVarName->str.c_str(), fontSize) + 2 * padding + 2 * 5;
	node->height = fontSize + 2 * padding + 2 * 5;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
	SetSingleLineTextPosition(node->myVarName, node->x + node->padding, node->y + node->padding);
}
void SetReadNodePosition(ReadNode* node, int x, int y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
	SetSingleLineTextPosition(node->myVarName, node->x + node->padding, node->y + node->padding);
}
void DrawReadNode(ReadNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, YELLOW);
	DrawSingleLineText(node->myVarName);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
	DrawLink(node->outPin, node->toPin);
}
void ResizeReadNode(ReadNode* node) {
	int varNameWidth = MeasureText(node->myVarName->str.c_str(), node->fontSize);
	node->width = 2 * node->padding + varNameWidth + 2 * 5;
	SetSingleLineTextPosition(node->myVarName, node->x + node->padding, node->y + node->padding);

	node->inPin.x = node->x + node->width / 2.0f;
	node->outPin.x = node->x + node->width / 2.0f;
}
void GetInputReadNode(ReadNode* node) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		node->myVarName->focused = IsSingleLineTextClicked(node->myVarName);
	}
	if (node->myVarName->focused) {
		char c = GetCharPressed();
		if (c != 0) {
			InsertCharSingleLineText(node->myVarName, c);
			ResizeReadNode(node);
		}
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
		if (node->myVarName->focused) {
			EraseCharSingleLineText(node->myVarName);
		}
		ResizeReadNode(node);
	}
}