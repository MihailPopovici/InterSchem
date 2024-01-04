#pragma once

#include "pin.h"

struct StartNode {
	int x, y, width, height; // TODO: float or int?
	int fontSize, padding;
	Pin outPin;
	Pin* toPin;
};

StartNode* NewStartNode(int padding, int fontSize, int x, int y);
void SetStartNodeSize(StartNode* node, int padding, int fontSize);
void SetStartNodePosition(StartNode* node, int x, int y);
void DrawStartNode(StartNode* node);