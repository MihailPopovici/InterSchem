#pragma once

#include "pin.h"
#include "singlelinetext.h";
#include "evalExpression.h";

struct DecisionNode {
	int id, index;
	int fontSize, padding;
	float x, y, width, height;

	SingleLineText* expression;

	Pin inPin;
	Pin outPinTrue;
	Pin outPinFalse;
	Pin* toPinTrue;
	Pin* toPinFalse;

	// TODO: func
};

DecisionNode* NewDecisionNode(int padding, int fontSize, float x, float y);
void SetDecisionNodeSize(DecisionNode* node, int padding, int fontSize);
void SetDecisionNodePosition(DecisionNode* node, float x, float y);
void DrawDecisionNode(DecisionNode* node);
int EvaluateDecisionNode(DecisionNode* node, Dictionary* dict);
void ResizeDecisionNode(DecisionNode* node);
void GetInputDecisionNode(DecisionNode* node);