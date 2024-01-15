#include "startNode.h"

#include "raylib.h"
#include <cstring>

StartNode* NewStartNode(int padding, int fontSize, int x, int y) {
	StartNode* p = new StartNode;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->outPin.type = output;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;
	p->outPin.ownerPtr = p;
	p->outPin.ownerType = NodeType_Start;

	p->toPin = nullptr;

	SetStartNodePosition(p, x, y);
	SetStartNodeSize(p, padding, fontSize);

	return p;
}
void SetStartNodeSize(StartNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText("Start", fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void SetStartNodePosition(StartNode* node, int x, int y) {
	node->x = x;
	node->y = y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void DrawStartNode(StartNode* node) {
	Rectangle rec;
	rec.x = node->x;
	rec.y = node->y;
	rec.width = node->width;
	rec.height = node->height;
	DrawRectangleRounded(rec, 1, 16, DARKGREEN);
	DrawText("Start", node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
	DrawLink(node->outPin, node->toPin);
}