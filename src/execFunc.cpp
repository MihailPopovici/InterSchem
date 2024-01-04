#include "execFunc.h"

#include "button.h"
#include <string>

void GetClickedNode(AnyNodeType& clickedNode, int mx, int my, NodeArrays& nodes) {
	if (nodes.startNode != nullptr) {
		if (mx >= nodes.startNode->x && mx <= nodes.startNode->x + nodes.startNode->width && my >= nodes.startNode->y && my <= nodes.startNode->y + nodes.startNode->height) {
			clickedNode.address = nodes.startNode;
			clickedNode.type = start;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.readNodes.size(); i++) {
		if (mx >= nodes.readNodes[i]->x && mx <= nodes.readNodes[i]->x + nodes.readNodes[i]->width && my >= nodes.readNodes[i]->y && my <= nodes.readNodes[i]->y + nodes.readNodes[i]->height) {
			clickedNode.address = nodes.readNodes[i];
			clickedNode.type = read;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.writeNodes.size(); i++) {
		if (mx >= nodes.writeNodes[i]->x && mx <= nodes.writeNodes[i]->x + nodes.writeNodes[i]->width && my >= nodes.writeNodes[i]->y && my <= nodes.writeNodes[i]->y + nodes.writeNodes[i]->height) {
			clickedNode.address = nodes.writeNodes[i];
			clickedNode.type = write;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.assignNodes.size(); i++) {
		if (mx >= nodes.assignNodes[i]->x && mx <= nodes.assignNodes[i]->x + nodes.assignNodes[i]->width && my >= nodes.assignNodes[i]->y && my <= nodes.assignNodes[i]->y + nodes.assignNodes[i]->height) {
			clickedNode.address = nodes.assignNodes[i];
			clickedNode.type = assign;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.decisionNodes.size(); i++) {
		if (mx >= nodes.decisionNodes[i]->x && mx <= nodes.decisionNodes[i]->x + nodes.decisionNodes[i]->width && my >= nodes.decisionNodes[i]->y && my <= nodes.decisionNodes[i]->y + nodes.decisionNodes[i]->height) {
			clickedNode.address = nodes.decisionNodes[i];
			clickedNode.type = decision;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.stopNodes.size(); i++) {
		if (mx >= nodes.stopNodes[i]->x && mx <= nodes.stopNodes[i]->x + nodes.stopNodes[i]->width && my >= nodes.stopNodes[i]->y && my <= nodes.stopNodes[i]->y + nodes.stopNodes[i]->height) {
			clickedNode.address = nodes.stopNodes[i];
			clickedNode.type = stop;
			return;
		}
	}

	clickedNode = { nullptr, noType };
}
void GetNextNodeInExecution(AnyNodeType& currentNode, ExecutionState& state, Dictionary* dict, MultiLineText* console) {
	if (currentNode.address == nullptr || currentNode.type == noType) {
		state = done; // TODO: add error variant
		return;
	}

	switch (currentNode.type) {
	case start:
		currentNode.type = ((StartNode*)currentNode.address)->toPin->ownerType;
		currentNode.address = ((StartNode*)currentNode.address)->toPin->ownerPtr;
		break;
	case read:
		currentNode.type = ((ReadNode*)currentNode.address)->toPin->ownerType;
		currentNode.address = ((ReadNode*)currentNode.address)->toPin->ownerPtr;
		break;
	case write:
		currentNode.type = ((WriteNode*)currentNode.address)->toPin->ownerType;
		currentNode.address = ((WriteNode*)currentNode.address)->toPin->ownerPtr;
		break;
	case assign: {
		currentNode.type = ((AssignNode*)currentNode.address)->toPin->ownerType;
		currentNode.address = ((AssignNode*)currentNode.address)->toPin->ownerPtr;
		break;
	}
	case decision: {
		int result = EvaluateDecisionNode((DecisionNode*)currentNode.address, dict);

		if (result == 0) {
			//TODO:
		}
		else if (result == 1) {
			currentNode.type = ((DecisionNode*)currentNode.address)->toPinTrue->ownerType;
			currentNode.address = ((DecisionNode*)currentNode.address)->toPinTrue->ownerPtr;
		}
		else {
			currentNode.type = ((DecisionNode*)currentNode.address)->toPinFalse->ownerType;
			currentNode.address = ((DecisionNode*)currentNode.address)->toPinFalse->ownerPtr;
		}
		break;
	}
	case stop: break;
	default: break;
	}

	if (currentNode.type == read) {
		state = waitingForInput;
	}
	else if (currentNode.type == stop) {
		state = done;
	}
	else {
		state = processing;
	}
}
void DrawSelectedNodeOptions(AnyNodeType& node, Button* del, Button* edit) { //TODO: asta n ar tb sa fie aici
	int* pos = (int*)node.address;
	int x = *pos, y = *(pos + 1), width = *(pos + 2), height = *(pos + 3);
	
	DrawRectangle(x - 2, y - 2, width + 4, height + 4, WHITE);

	SetButtonPosition(del, x + width + 7, y + (height - del->height) / 2);
	DrawButton(del);

	SetButtonPosition(edit, x + width + del->width + 12, y + (height - edit->height) / 2);
	DrawButton(edit);
}
void DragNode(AnyNodeType& node, int dx, int dy) {//TODO: asta n ar tb sa fie aici
	if (node.address == nullptr || node.type == noType) { // TODO: nu cred ca e necesar sa verifici
		return;
	}
	int mx = GetMouseX(), my = GetMouseY();
	if (node.type == start) {
		SetStartNodePosition((StartNode*)node.address, mx + dx, my + dy);
		return;
	}
	if (node.type == read) {
		SetReadNodePosition((ReadNode*)node.address, mx + dx, my + dy);
		return;
	}
	if (node.type == write) {
		SetWriteNodePosition((WriteNode*)node.address, mx + dx, my + dy);
		return;
	}
	if (node.type == assign) {
		SetAssignNodePosition((AssignNode*)node.address, mx + dx, my + dy);
		return;
	}
	if (node.type == decision) {
		SetDecisionNodePosition((DecisionNode*)node.address, mx + dx, my + dy);
		return;
	}
	else {
		SetStopNodePosition((StopNode*)node.address, mx + dx, my + dy);
		return;
	}
}
void GetClickedPin(Pin*& pin, NodeArrays& nodes) {
	pin = nullptr;
	Pin* p = nullptr;
	int mx = GetMouseX(), my = GetMouseY();
	int dx = 0, dy = 0;
	int bestDist = ~(1 << 31);
	int dist = 0;
	if (nodes.startNode != nullptr) {
		p = &nodes.startNode->outPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
	}
	for (ReadNode* n : nodes.readNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
		p = &n->outPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
	}
	for (WriteNode* n : nodes.writeNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
		p = &n->outPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
	}
	for (AssignNode* n : nodes.assignNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
		p = &n->outPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
	}
	for (DecisionNode* n : nodes.decisionNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
		p = &n->outPinTrue;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
		p = &n->outPinFalse;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
	}
	for (StopNode* n : nodes.stopNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
			pin = p;
		}
	}
}
int GetBestDistToPin(NodeArrays& nodes, int x, int y) {
	Pin* p = nullptr;
	//int mx = GetMouseX(), my = GetMouseY();
	int mx = x, my = y;
	int dx = 0, dy = 0;
	int bestDist = ~(1 << 31);
	int dist = 0;
	if (nodes.startNode != nullptr) {
		p = &nodes.startNode->outPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
	}
	for (ReadNode* n : nodes.readNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
		p = &n->outPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
	}
	for (WriteNode* n : nodes.writeNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
		p = &n->outPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
	}
	for (AssignNode* n : nodes.assignNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
		p = &n->outPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
	}
	for (DecisionNode* n : nodes.decisionNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
		p = &n->outPinTrue;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
		p = &n->outPinFalse;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
	}
	for (StopNode* n : nodes.stopNodes) {
		p = &n->inPin;
		dx = mx - p->x, dy = my - p->y;
		dist = dx * dx + dy * dy;
		if (dist <= 128 && dist < bestDist) {
			bestDist = dist;
		}
	}
	return bestDist;
}
void EraseNode(NodeArrays& nodes, AnyNodeType node) {
	if (node.type == start) {
		if (nodes.startNode == node.address) {
			delete nodes.startNode;
			nodes.startNode = nullptr;
			return;
		}
	}
	if (node.type == read) {
		EraseNodeLinks(nodes, &((ReadNode*)node.address)->inPin);
		for (unsigned i = 0; i < nodes.readNodes.size(); i++) {
			if (nodes.readNodes[i] == node.address) {
				delete nodes.readNodes[i];
				nodes.readNodes.erase(nodes.readNodes.begin() + i);
				return;
			}
		}
	}
	if (node.type == write) {
		EraseNodeLinks(nodes, &((WriteNode*)node.address)->inPin);
		for (unsigned i = 0; i < nodes.writeNodes.size(); i++) {
			if (nodes.writeNodes[i] == node.address) {
				delete nodes.writeNodes[i];
				nodes.writeNodes.erase(nodes.writeNodes.begin() + i);
				return;
			}
		}
	}
	if (node.type == assign) {
		EraseNodeLinks(nodes, &((AssignNode*)node.address)->inPin);
		for (unsigned i = 0; i < nodes.assignNodes.size(); i++) {
			if (nodes.assignNodes[i] == node.address) {
				delete nodes.assignNodes[i];
				nodes.assignNodes.erase(nodes.assignNodes.begin() + i);
				return;
			}
		}
	}
	if (node.type == decision) {
		EraseNodeLinks(nodes, &((DecisionNode*)node.address)->inPin);
		for (unsigned i = 0; i < nodes.decisionNodes.size(); i++) {
			if (nodes.decisionNodes[i] == node.address) {
				delete nodes.decisionNodes[i];
				nodes.decisionNodes.erase(nodes.decisionNodes.begin() + i);
				return;
			}
		}
	}
	if (node.type == stop) {
		EraseNodeLinks(nodes, &((StopNode*)node.address)->inPin);
		for (unsigned i = 0; i < nodes.stopNodes.size(); i++) {
			if (nodes.stopNodes[i] == node.address) {
				delete nodes.stopNodes[i];
				nodes.stopNodes.erase(nodes.stopNodes.begin() + i);
				return;
			}
		}
	}
}
void EraseNodeLinks(NodeArrays& nodes, Pin* inPin) {
	if (nodes.startNode != nullptr) {
		if (nodes.startNode->toPin == inPin) {
			nodes.startNode->toPin = nullptr;
		}
	}
	for (ReadNode* p : nodes.readNodes) {
		if (p->toPin == inPin) {
			p->toPin = nullptr;
		}
	}
	for (WriteNode* p : nodes.writeNodes) {
		if (p->toPin == inPin) {
			p->toPin = nullptr;
		}
	}
	for (AssignNode* p : nodes.assignNodes) {
		if (p->toPin == inPin) {
			p->toPin = nullptr;
		}
	}
	for (DecisionNode* p : nodes.decisionNodes) {
		if (p->toPinTrue == inPin) {
			p->toPinTrue = nullptr;
		}
		if (p->toPinFalse == inPin) {
			p->toPinFalse = nullptr;
		}
	}
}
void UpdateVariablesTable(NodeArrays& nodes, Dictionary* dict) {
	for (ReadNode* node : nodes.readNodes) {
		bool correctInput = isVariable(node->myVarName->str) && correctVariableName(node->myVarName->str);
		if (correctInput) {
			auto drow = GetDictionaryRow(dict, node->myVarName->str);
			if (drow == nullptr) {
				DictionaryRow* row = NewDictionaryRow();
				SetDictionaryRowData(row, node->myVarName->str, 0, 20, 5);
				AddDictionaryRow(dict, row);
				SetDictionaryRowColors(row, { 0,0,0,100 }, RAYWHITE);
			}
		}
		else {
			//TODO:popup
		}
	}
	for (AssignNode* node : nodes.assignNodes) {
		bool correctInput = isVariable(node->myVarName->str) && correctVariableName(node->myVarName->str);
		if (correctInput) {
			auto drow = GetDictionaryRow(dict, node->myVarName->str);
			if (drow == nullptr) {
				DictionaryRow* row = NewDictionaryRow();
				SetDictionaryRowData(row, node->myVarName->str, 0, 20, 5);
				AddDictionaryRow(dict, row);
				SetDictionaryRowColors(row, { 0,0,0,100 }, RAYWHITE);
			}
		}
		else {
			//TODO:popup
		}
	}
	ResizeDictionary(dict);
}