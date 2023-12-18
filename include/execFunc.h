#pragma once

#include "nodeComponents.h"

void GetClickedNodeID(NodeInfo& clickedNode, int mx, int my, NodeArrays& nodes) {
	if (clickedNode.id != -1) return;

	if (mx >= nodes.startNode->x && mx <= nodes.startNode->x + nodes.startNode->width && my >= nodes.startNode->y && my <= nodes.startNode->y + nodes.startNode->height) {
		clickedNode.id = nodes.startNode->id;
		clickedNode.index = 0;
		clickedNode.type = start;
		return;
	}
	for (unsigned i = 0; i < nodes.readNodes.size(); i++) {
		if (mx >= nodes.readNodes[i]->x && mx <= nodes.readNodes[i]->x + nodes.readNodes[i]->width && my >= nodes.readNodes[i]->y && my <= nodes.readNodes[i]->y + nodes.readNodes[i]->height) {
			clickedNode.id = nodes.readNodes[i]->id;
			clickedNode.index = i;
			clickedNode.type = read;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.writeNodes.size(); i++) {
		if (mx >= nodes.writeNodes[i]->x && mx <= nodes.writeNodes[i]->x + nodes.writeNodes[i]->width && my >= nodes.writeNodes[i]->y && my <= nodes.writeNodes[i]->y + nodes.writeNodes[i]->height) {
			clickedNode.id = nodes.writeNodes[i]->id;
			clickedNode.index = i;
			clickedNode.type = write;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.assignNodes.size(); i++) {
		if (mx >= nodes.assignNodes[i]->x && mx <= nodes.assignNodes[i]->x + nodes.assignNodes[i]->width && my >= nodes.assignNodes[i]->y && my <= nodes.assignNodes[i]->y + nodes.assignNodes[i]->height) {
			clickedNode.id = nodes.assignNodes[i]->id;
			clickedNode.index = i;
			clickedNode.type = assign;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.decisionNodes.size(); i++) {
		if (mx >= nodes.decisionNodes[i]->x && mx <= nodes.decisionNodes[i]->x + nodes.decisionNodes[i]->width && my >= nodes.decisionNodes[i]->y && my <= nodes.decisionNodes[i]->y + nodes.decisionNodes[i]->height) {
			clickedNode.id = nodes.decisionNodes[i]->id;
			clickedNode.index = i;
			clickedNode.type = decision;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.stopNodes.size(); i++) {
		if (mx >= nodes.stopNodes[i]->x && mx <= nodes.stopNodes[i]->x + nodes.stopNodes[i]->width && my >= nodes.stopNodes[i]->y && my <= nodes.stopNodes[i]->y + nodes.stopNodes[i]->height) {
			clickedNode.id = nodes.stopNodes[i]->id;
			clickedNode.index = i;
			clickedNode.type = stop;
			return;
		}
	}
}

void Execute(NodeArrays& nodes) {
	void* currentNode = nodes.startNode->toPin->owner;
	NodeType currentType = nodes.startNode->toPin->ownerType;
	while (currentType != stop) {
		switch (currentType) {
		//case start: break;
		case read: 
			ReadValue((ReadNode*)currentNode);

			currentType = ((ReadNode*)currentNode)->toPin->ownerType;
			currentNode = ((ReadNode*)currentNode)->toPin->owner;
			break;
		case write: 
			WriteValue((WriteNode*)currentNode);

			currentType = ((WriteNode*)currentNode)->toPin->ownerType;
			currentNode = ((WriteNode*)currentNode)->toPin->owner;
			break;
		case assign: 
			// TODO:

			currentType = ((AssignNode*)currentNode)->toPin->ownerType;
			currentNode = ((AssignNode*)currentNode)->toPin->owner;
			break;
		case decision: 
			// TODO: 
			// TODO: choose branch according to condition
			
			//currentType = ((DecisionNode*)currentNode)->toPinTrue->ownerType;
			//currentNode = ((DecisionNode*)currentNode)->toPinTrue->owner;
			//currentType = ((DecisionNode*)currentNode)->toPinFalse->ownerType;
			//currentNode = ((DecisionNode*)currentNode)->toPinFalse->owner;
			break;
		//case stop: break;
		default: break;
		}
	}
}