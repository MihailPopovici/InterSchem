#pragma once

#include "pin.h"
#include "singlelinetext.h"
#include "evalExpression.h"

struct ReadNode {
	int x, y, width, height;
	int fontSize, padding;
	Pin inPin;
	Pin outPin;
	Pin* toPin;

	SingleLineText* myVarName;
};

ReadNode* NewReadNode(int padding, int fontSize, int x, int y);
void SetReadNodeSize(ReadNode* node, int padding, int fontSize);
void SetReadNodePosition(ReadNode* node, int x, int y);
void DrawReadNode(ReadNode* node);
void ResizeReadNode(ReadNode* node);
void GetInputReadNode(ReadNode* node);