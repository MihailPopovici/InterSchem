#pragma once

// TODO: move this
#include <cstring>

#define PIN_RADIUS 4.0f

// TODO: ai grija sa nu iesi din vectori atunci cand adaugi un nod sau link

enum NodeType {
	none,
	start,
	read,
	write,
	assign,
	decision,
	stop
};

struct NodeInfo {
	int id, index;
	NodeType type;
};

struct Pin {
	int id;
	float x, y;
	float radius;
};

struct Link {
	Pin* from;
	Pin* to;
};

struct NodeLinks {
	unsigned nLinks = 0;
	Link vec[16] = { 0 };
};

struct StartNode {
	int id;
	char* label;
	int fontSize, padding;
	float x, y, width, height; // TODO: float or int?
	Pin outPin;
};

struct StopNode {
	int id;
	char* label;
	int fontSize, padding;
	float x, y, width, height; 
	Pin inPin;
};

struct ReadNode {
	int id;
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
	Pin outPin;
};

struct WriteNode {
	int id;
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
	Pin outPin;
};

struct AssignNode {
	int id;
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
	Pin outPin;
};

struct DecisionNode {
	int id;
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
	Pin outPinTrue;
	Pin outPinFalse;
};

struct NodeArrays {
	int globalPinID = 0, globalNodeID = 0;

	StartNode* startNode = 0;

	unsigned nReadNodes = 0;
	ReadNode* readNodes[16] = { 0 };

	unsigned nWriteNodes = 0;
	WriteNode* writeNodes[16] = { 0 };

	unsigned nAssignNodes = 0;
	AssignNode* assignNodes[16] = { 0 };

	unsigned nDecisionNodes = 0;
	DecisionNode* decisionNodes[16] = { 0 };

	unsigned nStopNodes = 0;
	StopNode* stopNodes[16] = { 0 };
};

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
}
StartNode* NewStartNode(int padding, int fontSize, float x, float y) {
	StartNode* p = new StartNode;
	p->id = 0;
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
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;

	SetStartNodePosition(p, x, y);
	SetStartNodeSize(p, padding, fontSize);

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
StopNode* NewStopNode(int padding, int fontSize, float x, float y) {
	StopNode* p = new StopNode;
	p->id = 0;
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
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;

	SetStopNodePosition(p, x, y);
	SetStopNodeSize(p, padding, fontSize);

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

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void DrawReadNode(ReadNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, YELLOW);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, BLACK);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
}
ReadNode* NewReadNode(int padding, int fontSize, float x, float y) {
	ReadNode* p = new ReadNode;
	p->id = 0;
	char* temp = new char[5];
	strcpy(temp, "Read");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = 0;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->outPin.id = 0;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;

	SetReadNodePosition(p, x, y);
	SetReadNodeSize(p, padding, fontSize);

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

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void DrawWriteNode(WriteNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, SKYBLUE);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
}
WriteNode* NewWriteNode(int padding, int fontSize, float x, float y) {
	WriteNode* p = new WriteNode;
	p->id = 0;
	char* temp = new char[6];
	strcpy(temp, "Write");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = 0;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->outPin.id = 0;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;

	SetWriteNodePosition(p, x, y);
	SetWriteNodeSize(p, padding, fontSize);

	return p;
}

void SetAssignNodeSize(AssignNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label, fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void SetAssignNodePosition(AssignNode* node, float x, float y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void DrawAssignNode(AssignNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, ORANGE);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, BLACK);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
}
AssignNode* NewAssignNode(int padding, int fontSize, float x, float y) {
	AssignNode* p = new AssignNode;
	p->id = 0;
	char* temp = new char[7];
	strcpy(temp, "Assign");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = 0;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->outPin.id = 0;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;

	SetAssignNodePosition(p, x, y);
	SetAssignNodeSize(p, padding, fontSize);

	return p;
}

void SetDecisionNodeSize(DecisionNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label, fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPinTrue.x = node->x;
	node->outPinTrue.y = node->y + node->height / 2.0f;

	node->outPinFalse.x = node->x + node->width;
	node->outPinFalse.y = node->y + node->height / 2.0f;
}
void SetDecisionNodePosition(DecisionNode* node, float x, float y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPinTrue.x = node->x;
	node->outPinTrue.y = node->y + node->height / 2.0f;

	node->outPinFalse.x = node->x + node->width;
	node->outPinFalse.y = node->y + node->height / 2.0f;
}
void DrawDecisionNode(DecisionNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, PURPLE);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPinTrue.x, node->outPinTrue.y, node->outPinTrue.radius, GRAY);
	DrawCircle(node->outPinFalse.x, node->outPinFalse.y, node->outPinFalse.radius, GRAY);
}
DecisionNode* NewDecisionNode(int padding, int fontSize, float x, float y) {
	DecisionNode* p = new DecisionNode;
	p->id = 0;
	char* temp = new char[9];
	strcpy(temp, "Decision");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = 0;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->outPinTrue.id = 0;
	p->outPinTrue.x = 0.0f;
	p->outPinTrue.y = 0.0f;
	p->outPinTrue.radius = PIN_RADIUS;
	p->outPinFalse.id = 0;
	p->outPinFalse.x = 0.0f;
	p->outPinFalse.y = 0.0f;
	p->outPinFalse.radius = PIN_RADIUS;

	SetDecisionNodePosition(p, x, y);
	SetDecisionNodeSize(p, padding, fontSize);

	return p;
}

void NewNode(NodeArrays& nodes, NodeType type, int padding, int fontSize, float x, float y) {
	switch (type) {
	case start: 
		nodes.startNode = NewStartNode(padding, fontSize, x, y);
		nodes.startNode->id = nodes.globalNodeID++;
		nodes.startNode->outPin.id = nodes.globalPinID++;
		break;
	case stop:
		nodes.stopNodes[nodes.nStopNodes] = NewStopNode(padding, fontSize, x, y);
		nodes.stopNodes[nodes.nStopNodes]->id = nodes.globalNodeID++;
		nodes.stopNodes[nodes.nStopNodes]->inPin.id = nodes.globalPinID++;
		nodes.nStopNodes++;
		break;
	case read:
		nodes.readNodes[nodes.nReadNodes] = NewReadNode(padding, fontSize, x, y);
		nodes.readNodes[nodes.nReadNodes]->id = nodes.globalNodeID++;
		nodes.readNodes[nodes.nReadNodes]->inPin.id = nodes.globalPinID++;
		nodes.readNodes[nodes.nReadNodes]->outPin.id = nodes.globalPinID++;
		nodes.nReadNodes++;
		break;
	case write: 
		nodes.writeNodes[nodes.nWriteNodes] = NewWriteNode(padding, fontSize, x, y);
		nodes.writeNodes[nodes.nWriteNodes]->id = nodes.globalNodeID++;
		nodes.writeNodes[nodes.nWriteNodes]->inPin.id = nodes.globalPinID++;
		nodes.writeNodes[nodes.nWriteNodes]->outPin.id = nodes.globalPinID++;
		nodes.nWriteNodes++;
		break;
	case assign:
		nodes.assignNodes[nodes.nAssignNodes] = NewAssignNode(padding, fontSize, x, y);
		nodes.assignNodes[nodes.nAssignNodes]->id = nodes.globalNodeID++;
		nodes.assignNodes[nodes.nAssignNodes]->inPin.id = nodes.globalPinID++;
		nodes.assignNodes[nodes.nAssignNodes]->outPin.id = nodes.globalPinID++;
		nodes.nAssignNodes++;
		break;
	case decision:
		nodes.decisionNodes[nodes.nDecisionNodes] = NewDecisionNode(padding, fontSize, x, y);
		nodes.decisionNodes[nodes.nDecisionNodes]->id = nodes.globalNodeID++;
		nodes.decisionNodes[nodes.nDecisionNodes]->inPin.id = nodes.globalPinID++;
		nodes.decisionNodes[nodes.nDecisionNodes]->outPinTrue.id = nodes.globalPinID++;
		nodes.decisionNodes[nodes.nDecisionNodes]->outPinFalse.id = nodes.globalPinID++;
		nodes.nDecisionNodes++;
		break;
	default:
		break;
	}
}

void NewLink(NodeLinks& links, Pin& from, Pin& to) {
	Link link;
	link.from = &from;
	link.to = &to;
	links.vec[links.nLinks] = link;
	links.nLinks++;
}
void DrawLink(Link link) {
	DrawLineEx({ link.from->x, link.from->y }, { link.to->x, link.to->y }, 2.0f, GRAY);
}