#include "startNode.h"

#include "raylib.h"
#include <cstring>

StartNode* NewStartNode(int padding, int fontSize, float x, float y) {
	StartNode* p = new StartNode;
	p->id = -1;
	char* temp = new char[6];
	strcpy(temp, "Start");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->outPin.id = 0;
	p->outPin.type = output;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;
	p->outPin.owner = p;
	p->outPin.ownerType = start;

	p->toPin = nullptr;

	SetStartNodePosition(p, x, y);
	SetStartNodeSize(p, padding, fontSize);

	return p;
}
void SetStartNodeSize(StartNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label, fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void SetStartNodePosition(StartNode* node, float x, float y) {
	node->x = x;
	node->y = y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void DrawStartNode(StartNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, DARKGREEN);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
	DrawLink(node->outPin, node->toPin);
}