#pragma once

#include "pin.h"

struct StopNode {
	int x, y, width, height;
	int fontSize, padding;
	Pin inPin;
};

StopNode* NewStopNode(int padding, int fontSize, int x, int y);
void SetStopNodeSize(StopNode* node, int padding, int fontSize);
void SetStopNodePosition(StopNode* node, int x, int y);
void DrawStopNode(StopNode* node);