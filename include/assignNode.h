#pragma once

#include "pin.h"
#include <string>
#include "singlelinetext.h"

struct AssignNode {
	int id, index;
	std::string label;
	int fontSize, padding;
	float x, y, width, height;


	Pin inPin;
	Pin outPin;
	Pin* toPin;

	std::string* myVarName;
	int* myVarValue;
	std::string expression;
	// pe nod
	// myVarName = expression
	// result = eval(expression)
	// myVarValue = result
};

AssignNode* NewAssignNode(int padding, int fontSize, float x, float y);
void SetAssignNodeSize(AssignNode* node, int padding, int fontSize);
void SetAssignNodePosition(AssignNode* node, float x, float y);
void DrawAssignNode(AssignNode* node);
void LinkAssignNodeVar(AssignNode* node, std::string* name, int* val);
void SetAssignNodeExpression(AssignNode* node, std::string expression);
void EvaluateAssignNode(AssignNode* node);