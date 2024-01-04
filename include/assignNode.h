#pragma once

#include "pin.h"
#include <string>
#include "singlelinetext.h"
#include "dictionary.h"

struct AssignNode {
	int x, y, width, height;
	int fontSize, padding;
	Pin inPin;
	Pin outPin;
	Pin* toPin;

	SingleLineText* myVarName;
	SingleLineText* expression;
};

AssignNode* NewAssignNode(int padding, int fontSize, int x, int y);
void SetAssignNodeSize(AssignNode* node, int padding, int fontSize);
void SetAssignNodePosition(AssignNode* node, int x, int y);
void DrawAssignNode(AssignNode* node);
void EvaluateAssignNode(AssignNode* node, Dictionary* dict);
void ResizeAssignNode(AssignNode* node);
void GetInputAssignNode(AssignNode* node);