#pragma once

#include "pin.h"

struct DecisionNode {
	int id, index;
	char* label;
	int fontSize, padding;
	float x, y, width, height;

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