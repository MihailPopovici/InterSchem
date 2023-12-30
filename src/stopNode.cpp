#include "stopNode.h"

#include "raylib.h"
#include <cstring>

StopNode* NewStopNode(int padding, int fontSize, float x, float y) {
	StopNode* p = new StopNode;
	p->id = -1;
	p->index = -1;
	char* temp = new char[5];
	strcpy(temp, "Stop");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

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
	p->inPin.ownerType = stop;

	SetStopNodePosition(p, x, y);
	SetStopNodeSize(p, padding, fontSize);

	return p;
}
void SetStopNodeSize(StopNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label, fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;
}
void SetStopNodePosition(StopNode* node, float x, float y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;
}
void DrawStopNode(StopNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, RED);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
}