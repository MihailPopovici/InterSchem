#pragma once

#include "codeComponents.h"

int GetClickedNodeID(float x, float y, int clickedNodeID, ReadNode* readNodes[]) {
	if (clickedNodeID != -1) return clickedNodeID;
	for (unsigned i = 0; readNodes[i] != nullptr; i++) {
		if ((x >= readNodes[i]->x && x <= readNodes[i]->x + readNodes[i]->width) && (y >= readNodes[i]->y && y <= readNodes[i]->y + readNodes[i]->height)) {
			return readNodes[i]->id;
		}
	}
}