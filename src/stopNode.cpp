#include "stopNode.h"

#include "raylib.h"
#include <cstring>

StopNode* NewStopNode(int padding, int fontSize, int x, int y) {
	StopNode* p = new StopNode;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.type = input;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->inPin.ownerPtr = p;
	p->inPin.ownerType = stop;

	SetStopNodePosition(p, x, y);
	SetStopNodeSize(p, padding, fontSize);

	return p;
}
void SetStopNodeSize(StopNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText("Stop", fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;
}
void SetStopNodePosition(StopNode* node, int x, int y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;
}
void DrawStopNode(StopNode* node) {
	Rectangle rec;
	rec.x = node->x;
	rec.y = node->y;
	rec.width = node->width;
	rec.height = node->height;
	DrawRectangleRounded(rec, 1, 16, RED);
	DrawText("Stop", node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
}