#pragma once

#include "pin.h"
#include "singlelinetext.h"
#include "evalExpression.h"

struct WriteNode {
	int id, index;
	int fontSize, padding;
	float x, y, width, height;

	SingleLineText* expression;

	Pin inPin;
	Pin outPin;
	Pin* toPin;

	int myVarValue;
};

WriteNode* NewWriteNode(int padding, int fontSize, float x, float y);
void SetWriteNodeSize(WriteNode* node, int padding, int fontSize);
void SetWriteNodePosition(WriteNode* node, float x, float y);
void DrawWriteNode(WriteNode* node);
void LinkWriteNodeVar(WriteNode* node, int* x);
int GetWriteNodeVarValue(WriteNode* node);
void EvaluateWriteNode(WriteNode* node, Dictionary* dict);
void ResizeWriteNode(WriteNode* node);
void GetInputWriteNode(WriteNode* node);