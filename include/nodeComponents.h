#pragma once

#include "startNode.h"
#include "stopNode.h"
#include "readNode.h"
#include "writeNode.h"
#include "assignNode.h"
#include "decisionNode.h"
#include "nodeGeneral.h"

#include <vector>

struct NodeArrays {
	int globalPinID = 0, globalNodeID = 0;

	StartNode* startNode = 0;
	std::vector<StopNode*> stopNodes;
	std::vector<ReadNode*> readNodes;
	std::vector<WriteNode*> writeNodes;
	std::vector<AssignNode*> assignNodes;
	std::vector<DecisionNode*> decisionNodes;
};

void NewNode(NodeArrays& nodes, NodeType type, int padding, int fontSize, float x, float y) {
	switch (type) {
	case start: 
		nodes.startNode = NewStartNode(padding, fontSize, x, y);
		nodes.startNode->id = nodes.globalNodeID++;
		nodes.startNode->outPin.id = nodes.globalPinID++;
		break;
	case stop:
	{
		StopNode* s = NewStopNode(padding, fontSize, x, y);
		s->id = nodes.globalNodeID++;
		s->index = nodes.stopNodes.size();
		s->inPin.id = nodes.globalPinID++;
		nodes.stopNodes.push_back(s);
		break;
	}
	case read:
	{
		ReadNode* r = NewReadNode(padding, fontSize, x, y);
		r->id = nodes.globalNodeID++;
		r->index = nodes.readNodes.size();
		r->inPin.id = nodes.globalPinID++;
		nodes.readNodes.push_back(r);
		break;
	}
	case write: 
	{
		WriteNode* w = NewWriteNode(padding, fontSize, x, y);
		w->id = nodes.globalNodeID++;
		w->index = nodes.writeNodes.size();
		w->inPin.id = nodes.globalPinID++;
		nodes.writeNodes.push_back(w);
		break;
	}
	case assign:
	{
		AssignNode* a = NewAssignNode(padding, fontSize, x, y);
		a->id = nodes.globalNodeID++;
		a->index = nodes.assignNodes.size();
		a->inPin.id = nodes.globalPinID++;
		nodes.assignNodes.push_back(a);
		break;
	}
	case decision:
	{
		DecisionNode* d = NewDecisionNode(padding, fontSize, x, y);
		d->id = nodes.globalNodeID++;
		d->index = nodes.decisionNodes.size();
		d->outPinTrue.id = nodes.globalPinID++;
		d->outPinFalse.id = nodes.globalPinID++;
		nodes.decisionNodes.push_back(d);
		break;
	}
	default:
		break;
	}
}