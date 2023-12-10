#pragma once

// TODO: move this
#include <cstring>

#define PIN_RADIUS 4.0f

struct Pin {
	int id;
	float x, y;
	float radius;
};

struct Link {
	Pin* start;
	Pin* end;
};

struct StartNode {
	char* label;
	int fontSize, padding;
	float x, y, width, height; // TODO: float or int?
	Pin outPin;
};

struct StopNode {
	char* label;
	int fontSize, padding;
	float x, y, width, height; // TODO: float or int?
	Pin inPin;
};

struct ReadNode {
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
	Pin outPin;
};

struct WriteNode {
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
	Pin outPin;
};

struct AssignNode {

};

struct Decision {

};

StartNode* NewStartNode(unsigned& globalID) {
	StartNode* p = new StartNode;
	char* temp = new char[6];
	strcpy(temp, "Start");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->outPin.id = globalID++;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;

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
}
void DrawStartNode(StartNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, DARKGREEN);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
}

StopNode* NewStopNode(unsigned& globalID) {
	StopNode* p = new StopNode;
	char* temp = new char[5];
	strcpy(temp, "Stop");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = globalID++;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;

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
}
void DrawStopNode(StopNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, RED);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
}

ReadNode* NewReadNode(unsigned& globalID) {
	ReadNode* p = new ReadNode;
	char* temp = new char[5];
	strcpy(temp, "Read");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = globalID++;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->outPin.id = globalID++;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;

	return p;
}
void SetReadNodeSize(ReadNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label, fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void SetReadNodePosition(ReadNode* node, float x, float y) {
	node->x = x;
	node->y = y;
}
void DrawReadNode(ReadNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, YELLOW);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, BLACK);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
}

WriteNode* NewWriteNode(unsigned& globalID) {
	WriteNode* p = new WriteNode;
	char* temp = new char[6];
	strcpy(temp, "Write");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = globalID++;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->outPin.id = globalID++;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;

	return p;
}
void SetWriteNodeSize(WriteNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label, fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void SetWriteNodePosition(WriteNode* node, float x, float y) {
	node->x = x;
	node->y = y;
}
void DrawWriteNode(WriteNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, SKYBLUE);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
}

Link NewLink(StartNode* start, StopNode* stop, unsigned& nLinks) {
	Link link;
	link.start = &start->outPin;
	link.end = &stop->inPin;
	nLinks++;
	return link;
}
Link NewLink(StartNode* start, ReadNode* read, unsigned& nLinks) {
	Link link;
	link.start = &start->outPin;
	link.end = &read->inPin;
	nLinks++;
	return link;
}
Link NewLink(ReadNode* read, StopNode* stop, unsigned& nLinks) {
	Link link;
	link.start = &read->outPin;
	link.end = &stop->inPin;
	nLinks++;
	return link;
}
Link NewLink(StartNode* start, WriteNode* write, unsigned& nLinks) {
	Link link;
	link.start = &start->outPin;
	link.end = &write->inPin;
	nLinks++;
	return link;
}
Link NewLink(WriteNode* write, StopNode* stop, unsigned& nLinks) {
	Link link;
	link.start = &write->outPin;
	link.end = &stop->inPin;
	nLinks++;
	return link;
}
Link NewLink(ReadNode* read, WriteNode* write, unsigned& nLinks) {
	Link link;
	link.start = &read->outPin;
	link.end = &write->inPin;
	nLinks++;
	return link;
}
void DrawLink(Link link) {
	DrawLineEx({ link.start->x, link.start->y }, { link.end->x, link.end->y }, 2.0f, GRAY);
}