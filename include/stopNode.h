#pragma once

#include "pin.h"

struct StopNode {
	int id, index;
	char* label;
	int fontSize, padding;
	float x, y, width, height;

	Pin inPin;
};

StopNode* NewStopNode(int padding, int fontSize, float x, float y);
void SetStopNodeSize(StopNode* node, int padding, int fontSize);
void SetStopNodePosition(StopNode* node, float x, float y);
void DrawStopNode(StopNode* node);