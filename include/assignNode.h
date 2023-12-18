#pragma once

#include "pin.h"

struct AssignNode {
	int id, index;
	char* label;
	int fontSize, padding;
	float x, y, width, height;

	Pin inPin;
	Pin outPin;
	Pin* toPin;

	// TODO: var and expr
};

AssignNode* NewAssignNode(int padding, int fontSize, float x, float y);
void SetAssignNodeSize(AssignNode* node, int padding, int fontSize);
void SetAssignNodePosition(AssignNode* node, float x, float y);
void DrawAssignNode(AssignNode* node);