#include "nodeComponents.h"

AnyNodeType NewNode(NodeArrays& nodes, NodeType type, int padding, int fontSize, float x, float y) {
	AnyNodeType res = { nullptr, noType };
	if (type == start) {
		res.address = nodes.startNode = NewStartNode(padding, fontSize, x, y);
		nodes.startNode->id = nodes.globalNodeID++;
		nodes.startNode->outPin.id = nodes.globalPinID++;
		res.type = start;
		return res;
	}
	if(type == stop) {
		StopNode* s = NewStopNode(padding, fontSize, x, y);
		s->id = nodes.globalNodeID++;
		s->index = nodes.stopNodes.size();
		s->inPin.id = nodes.globalPinID++;
		nodes.stopNodes.push_back(s);
		res.address = s;
		res.type = stop;
		return res;
	}
	if(type == read) {
		ReadNode* r = NewReadNode(padding, fontSize, x, y);
		r->id = nodes.globalNodeID++;
		r->index = nodes.readNodes.size();
		r->inPin.id = nodes.globalPinID++;
		nodes.readNodes.push_back(r);
		res.address = r;
		res.type = read;
		return res;
	}
	if (type == write)	{
		WriteNode* w = NewWriteNode(padding, fontSize, x, y);
		w->id = nodes.globalNodeID++;
		w->index = nodes.writeNodes.size();
		w->inPin.id = nodes.globalPinID++;
		nodes.writeNodes.push_back(w);
		res.address = w;
		res.type = write;
		return res;
	}
	if (type == assign) {
		AssignNode* a = NewAssignNode(padding, fontSize, x, y);
		a->id = nodes.globalNodeID++;
		a->index = nodes.assignNodes.size();
		a->inPin.id = nodes.globalPinID++;
		nodes.assignNodes.push_back(a);
		res.address = a;
		res.type = assign;
		return res;
	}
	if (type == decision) {
		DecisionNode* d = NewDecisionNode(padding, fontSize, x, y);
		d->id = nodes.globalNodeID++;
		d->index = nodes.decisionNodes.size();
		d->outPinTrue.id = nodes.globalPinID++;
		d->outPinFalse.id = nodes.globalPinID++;
		nodes.decisionNodes.push_back(d);
		res.address = d;
		res.type = decision;
		return res;
	}
	return res;
}
void DrawNodes(NodeArrays& nodes) {
	if (nodes.startNode != nullptr) {
		DrawStartNode(nodes.startNode);
	}
	for (StopNode* p : nodes.stopNodes) {
		DrawStopNode(p);
	}
	for (ReadNode* p : nodes.readNodes) {
		DrawReadNode(p);
	}
	for (WriteNode* p : nodes.writeNodes) {
		DrawWriteNode(p);
	}
	for (AssignNode* p : nodes.assignNodes) {
		DrawAssignNode(p);
	}
	for (DecisionNode* p : nodes.decisionNodes) {
		DrawDecisionNode(p);
	}
}
void CleanupNodes(NodeArrays& nodes) {
	if (nodes.startNode != nullptr) {
		delete nodes.startNode;
	}
	for (ReadNode* p : nodes.readNodes) {
		if (p != nullptr) {
			delete p;
		}
	}
	nodes.readNodes.clear();
	for (WriteNode* p : nodes.writeNodes) {
		if (p != nullptr) {
			delete p;
		}
	}
	nodes.writeNodes.clear();
	for (AssignNode* p : nodes.assignNodes) {
		if (p != nullptr) {
			delete p;
		}
	}
	nodes.assignNodes.clear();
	for (DecisionNode* p : nodes.decisionNodes) {
		if (p != nullptr) {
			delete p;
		}
	}
	nodes.decisionNodes.clear();
	for (StopNode* p : nodes.stopNodes) {
		if (p != nullptr) {
			delete p;
		}
	}
	nodes.stopNodes.clear();
}