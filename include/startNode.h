#pragma once

#include "pin.h"

struct StartNode {
	int id;
	char* label;
	int fontSize, padding;
	float x, y, width, height; // TODO: float or int?

	Pin outPin;
	Pin* toPin;
};

StartNode* NewStartNode(int padding, int fontSize, float x, float y);
void SetStartNodeSize(StartNode* node, int padding, int fontSize);
void SetStartNodePosition(StartNode* node, float x, float y);
void DrawStartNode(StartNode* node);