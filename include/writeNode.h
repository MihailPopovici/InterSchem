#pragma once

#include "pin.h"
#include "singlelinetext.h"
#include "evalExpression.h"

struct WriteNode {
	int x, y, width, height;
	int fontSize, padding;
	Pin inPin;
	Pin outPin;
	Pin* toPin;

	SingleLineText* expression;
};

WriteNode* NewWriteNode(int padding, int fontSize, int x, int y);
void SetWriteNodeSize(WriteNode* node, int padding, int fontSize);
void SetWriteNodePosition(WriteNode* node, int x, int y);
void DrawWriteNode(WriteNode* node);
float EvaluateWriteNode(WriteNode* node, Dictionary* dict); // TODO: functiile astea ar tb sa returneze rezultatul direct, iar alta functie sa verifice corectitudinea fiecarei expresii din fiecare nod inainte de a executa
void ResizeWriteNode(WriteNode* node);
void GetInputWriteNode(WriteNode* node);