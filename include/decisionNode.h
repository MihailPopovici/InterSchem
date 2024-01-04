#pragma once

#include "pin.h"
#include "singlelinetext.h";
#include "evalExpression.h";

struct DecisionNode {
	int x, y, width, height;
	int fontSize, padding;
	Pin inPin;
	Pin outPinTrue;
	Pin outPinFalse;
	Pin* toPinTrue;
	Pin* toPinFalse;

	SingleLineText* expression;
};

DecisionNode* NewDecisionNode(int padding, int fontSize, int x, int y);
void SetDecisionNodeSize(DecisionNode* node, int padding, int fontSize);
void SetDecisionNodePosition(DecisionNode* node, int x, int y);
void DrawDecisionNode(DecisionNode* node);
int EvaluateDecisionNode(DecisionNode* node, Dictionary* dict);
void ResizeDecisionNode(DecisionNode* node);
void GetInputDecisionNode(DecisionNode* node);