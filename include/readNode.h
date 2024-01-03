#pragma once

#include "pin.h"
#include "singlelinetext.h"
#include "evalExpression.h"
#include <string>

struct ReadNode {
	int id, index;
	int fontSize, padding;
	float x, y, width, height;

	SingleLineText* myVarName;

	Pin inPin;
	Pin outPin;
	Pin* toPin;

	std::string* old_myVarName;
	int* myVarValue;
};

ReadNode* NewReadNode(int padding, int fontSize, float x, float y);
void SetReadNodeSize(ReadNode* node, int padding, int fontSize);
void SetReadNodePosition(ReadNode* node, float x, float y);
void DrawReadNode(ReadNode* node);
void LinkReadNodeVar(ReadNode* node, std::string* name, int* val);
void SetReadNodeVarValue(ReadNode* node, int x);

void ResizeReadNode(ReadNode* node);
void GetInputReadNode(ReadNode* node);