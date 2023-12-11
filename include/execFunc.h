#pragma once

#include "codeComponents.h"

enum NodeType {
	none,
	start,
	read,
	write,
	assign,
	decision,
	stop
};

struct ClickedNodeInfo {
	int id, index;
	NodeType type;
};

void GetClickedNodeID(ClickedNodeInfo& cNode, int mx, int my, StartNode* startNode, ReadNode* readNodes[], WriteNode* writeNodes[], AssignNode* assignNodes[], /*DecisionNode* decisionNodes[], */ StopNode* stopNodes[]) {
	if (cNode.id != -1) return;

	if (mx >= startNode->x && mx <= startNode->x + startNode->width && my >= startNode->y && my <= startNode->y + startNode->height) {
		cNode.id = startNode->id;
		cNode.index = 0;
		cNode.type = start;
		return;
	}
	for (unsigned i = 0; readNodes[i] != nullptr; i++) {
		if (mx >= readNodes[i]->x && mx <= readNodes[i]->x + readNodes[i]->width && my >= readNodes[i]->y && my <= readNodes[i]->y + readNodes[i]->height) {
			cNode.id = readNodes[i]->id;
			cNode.index = i;
			cNode.type = read;
			return;
		}
	}
	for (unsigned i = 0; writeNodes[i] != nullptr; i++) {
		if (mx >= writeNodes[i]->x && mx <= writeNodes[i]->x + writeNodes[i]->width && my >= writeNodes[i]->y && my <= writeNodes[i]->y + writeNodes[i]->height) {
			cNode.id = writeNodes[i]->id;
			cNode.index = i;
			cNode.type = write;
			return;
		}
	}
	for (unsigned i = 0; assignNodes[i] != nullptr; i++) {
		if (mx >= assignNodes[i]->x && mx <= assignNodes[i]->x + assignNodes[i]->width && my >= assignNodes[i]->y && my <= assignNodes[i]->y + assignNodes[i]->height) {
			cNode.id = assignNodes[i]->id;
			cNode.index = i;
			cNode.type = assign;
			return;
		}
	}
	// decision
	for (unsigned i = 0; stopNodes[i] != nullptr; i++) {
		if (mx >= stopNodes[i]->x && mx <= stopNodes[i]->x + stopNodes[i]->width && my >= stopNodes[i]->y && my <= stopNodes[i]->y + stopNodes[i]->height) {
			cNode.id = stopNodes[i]->id;
			cNode.index = i;
			cNode.type = stop;
			return;
		}
	}
}