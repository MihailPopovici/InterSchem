#include "writeNode.h"

#include "raylib.h"
#include <cstring>
#include <iostream>

WriteNode* NewWriteNode(int padding, int fontSize, int x, int y) {
	WriteNode* p = new WriteNode;
	p->fontSize = 0;
	p->padding = 0;

	p->expression = NewSingleLineText();
	SetSingleLineTextColors(p->expression, { 100, 100, 100, 70 }, WHITE);
	SetSingleLineTextPadding(p->expression, 5);
	SetSingleLineTextFontSize(p->expression, 20);
	SetSingleLineTextPosition(p->expression, 0, 0);
	p->expression->visible = true;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.type = input;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->inPin.ownerPtr = p;
	p->inPin.ownerType = NodeType_Write;

	p->outPin.type = output;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;
	p->outPin.ownerPtr = p;
	p->outPin.ownerType = NodeType_Write;

	p->toPin = nullptr;

	SetWriteNodePosition(p, x, y);
	SetWriteNodeSize(p, padding, fontSize);

	return p;
}
void SetWriteNodeSize(WriteNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->expression->str.c_str(), fontSize) + 2 * padding + 2 * 5;
	node->height = fontSize + 2 * padding+2*5;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;

	SetSingleLineTextPosition(node->expression, node->x + node->padding, node->y + node->padding);
}
void SetWriteNodePosition(WriteNode* node, int x, int y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;

	SetSingleLineTextPosition(node->expression, node->x + node->padding, node->y + node->padding);
}
void DrawWriteNode(WriteNode* node) {
	Vector2 v1;
	v1.x = node->x;
	v1.y = node->y + node->height;
	Vector2 v2;
	v2.x = node->x + 20;
	v2.y = node->y + node->height;
	Vector2 v3;
	v3.x = node->x + 20;
	v3.y = node->y;
	DrawTriangle(v1, v2, v3, SKYBLUE);
	DrawRectangle(node->x + 20, node->y, node->width - 40, node->height, SKYBLUE);
	v1.x = node->x + node->width - 20;
	v1.y = node->y + node->height;
	v2.x = node->x + node->width;
	v2.y = node->y + node->height;
	v3.x = node->x + node->width - 20;
	v3.y = node->y;
	DrawTriangle(v1, v2, v3, SKYBLUE);
	DrawSingleLineText(node->expression);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
	DrawLink(node->outPin, node->toPin);
}
float EvaluateWriteNode(WriteNode* node, Dictionary* dict) {
	int err = 0;
	float result = evaluate(node->expression->str, dict, err);
	return result;
}
void ResizeWriteNode(WriteNode* node) {
	int expressionWidth = MeasureText(node->expression->str.c_str(), node->fontSize);
	node->width = 2 * node->padding + expressionWidth + 2 * 5;
	SetSingleLineTextPosition(node->expression, node->x + node->padding, node->y + node->padding);

	node->inPin.x = node->x + node->width / 2.0f;
	node->outPin.x = node->x + node->width / 2.0f;
	
}
void GetInputWriteNode(WriteNode* node) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		node->expression->focused = IsSingleLineTextClicked(node->expression);
	}
	if (node->expression->focused) {
		char c = GetCharPressed();
		if (c != 0) {
			InsertCharSingleLineText(node->expression, c);
			ResizeWriteNode(node);
		}
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
		if (node->expression->focused) {
			EraseCharSingleLineText(node->expression);
		}
		ResizeWriteNode(node);
	}
}