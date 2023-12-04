#pragma once

// TODO: move this
#include <cstring>

struct StartNode {
	char* label;
	float x, y, width, height; // TODO: float or int?
	int out;
};

struct StopNode {

};

struct ReadNode {

};

struct WriteNode {
	
};

struct AssignNode {

};

struct Decision {
	char* label;
	int* in;
	int outTrue;
	int outFalse;
};

StartNode* NewStartNode() {
	StartNode* p = new StartNode;
	p->label = nullptr;
	p->x = -1;
	p->y = -1;
	p->width = -1;
	p->height = -1;
	p->out = -1;
	return p;
}

void SetLabel(StartNode* p, const char* label) { // TODO: const ?
	p->label = new char[strlen(label) + 1];
	strcpy(p->label, label);
}

Decision* NewDecisionNode() {
	Decision* p = new Decision;
	p->label = nullptr;
	p->in = nullptr;
	p->outTrue = -1;
	p->outFalse = -1;
	return p;
}