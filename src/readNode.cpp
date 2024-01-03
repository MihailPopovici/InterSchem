#include "readNode.h"

#include "raylib.h"
#include <cstring>
#include <iostream>

ReadNode* NewReadNode(int padding, int fontSize, float x, float y) {
	ReadNode* p = new ReadNode;
	p->id = -1;
	p->index = -1;
	p->fontSize = 0;
	p->padding = 0;

	p->varName = NewSingleLineText();
	SetSingleLineTextColors(p->varName, { 100, 100, 100, 70 }, WHITE);
	SetSingleLineTextPadding(p->varName, 5);
	SetSingleLineTextFontSize(p->varName, 20);
	SetSingleLineTextPosition(p->varName, 0, 0);

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = 0;
	p->inPin.type = input;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->inPin.owner = p;
	p->inPin.ownerType = read;

	p->outPin.id = 0;
	p->outPin.type = output;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;
	p->outPin.owner = p;
	p->outPin.ownerType = read;

	p->toPin = nullptr;

	p->myVarValue = nullptr;
	p->myVarName = nullptr;

	SetReadNodePosition(p, x, y);
	SetReadNodeSize(p, padding, fontSize);

	return p;
}
void SetReadNodeSize(ReadNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->varName->str.c_str(), fontSize) + 2 * padding + 2 * 5;
	node->height = fontSize + 2 * padding + 2 * 5;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
	SetSingleLineTextPosition(node->varName, node->x + node->padding, node->y + node->padding);
}
void SetReadNodePosition(ReadNode* node, float x, float y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
	SetSingleLineTextPosition(node->varName, node->x + node->padding, node->y + node->padding);
}
void DrawReadNode(ReadNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, YELLOW);
	DrawSingleLineText(node->varName);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
	DrawLink(node->outPin, node->toPin);
}
void LinkReadNodeVar(ReadNode* node, std::string* name, int* val) {
	node->myVarName = name;
	node->myVarValue = val;
}
void SetReadNodeVarValue(ReadNode* node, int x) {
	*(node->myVarValue) = x;
}


void EvaluateReadNode(ReadNode* node, Dictionary* dict) {
	auto drow = GetDictionaryRow(dict, node->varName->str);
	SetDictionaryRowValue(drow, 10);//TODO:set value to input from console
}
void ResizeReadNode(ReadNode* node) {
	int varNameWidth = MeasureText(node->varName->str.c_str(), node->fontSize);
	node->width = 2 * node->padding + varNameWidth + 2 * 5;
	SetSingleLineTextPosition(node->varName, node->x + node->padding, node->y + node->padding);

	node->inPin.x = node->x + node->width / 2.0f;
	node->outPin.x = node->x + node->width / 2.0f;
}
void GetInputReadNode(ReadNode* node) {
	//std::cout << node->varName << " " << node->expression << "\n";
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		node->varName->focused = IsSingleLineTextClicked(node->varName);
	}
	if (node->varName->focused) {
		char c = GetCharPressed();
		if (c != 0) {
			InsertCharSingleLineText(node->varName, c);
			ResizeReadNode(node);
		}
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
		if (node->varName->focused) {
			EraseCharSingleLineText(node->varName);
		}
		ResizeReadNode(node);
	}
}