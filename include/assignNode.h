#pragma once

#include "pin.h"
#include <string>
#include "singlelinetext.h"
#include "dictionary.h"

struct AssignNode {
	int id, index;
	int fontSize, padding;
	float x, y, width, height;

	SingleLineText* myVarName;
	SingleLineText* expression;

	Pin inPin;
	Pin outPin;
	Pin* toPin;

	// pe nod
	// old_myVarName = expression
	// result = eval(expression)
	// myVarValue = result
};

AssignNode* NewAssignNode(int padding, int fontSize, float x, float y);
void SetAssignNodeSize(AssignNode* node, int padding, int fontSize);
void SetAssignNodePosition(AssignNode* node, int x, int y);
void DrawAssignNode(AssignNode* node);
void LinkAssignNodeVar(AssignNode* node, std::string* name, int* val);
void SetAssignNodeExpression(AssignNode* node, std::string expression);
void EvaluateAssignNode(AssignNode* node, Dictionary* dict);
void ResizeAssignNode(AssignNode* node);
void GetInputAssignNode(AssignNode* node);