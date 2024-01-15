#include "nodeComponents.h"

AnyNodeType NewNode(NodeArrays& nodes, NodeType type, int padding, int fontSize, float x, float y) {
	AnyNodeType res = { nullptr, NodeType_NoType };
	if (type == NodeType_Start) {
		res.address = nodes.startNode = NewStartNode(padding, fontSize, x, y);
		res.type = NodeType_Start;
		return res;
	}
	if(type == NodeType_Stop) {
		StopNode* s = NewStopNode(padding, fontSize, x, y);
		nodes.stopNodes.push_back(s);
		res.address = s;
		res.type = NodeType_Stop;
		return res;
	}
	if(type == NodeType_Read) {
		ReadNode* r = NewReadNode(padding, fontSize, x, y);
		nodes.readNodes.push_back(r);
		res.address = r;
		res.type = NodeType_Read;
		return res;
	}
	if (type == NodeType_Write)	{
		WriteNode* w = NewWriteNode(padding, fontSize, x, y);
		nodes.writeNodes.push_back(w);
		res.address = w;
		res.type = NodeType_Write;
		return res;
	}
	if (type == NodeType_Assign) {
		AssignNode* a = NewAssignNode(padding, fontSize, x, y);
		nodes.assignNodes.push_back(a);
		res.address = a;
		res.type = NodeType_Assign;
		return res;
	}
	if (type == NodeType_Decision) {
		DecisionNode* d = NewDecisionNode(padding, fontSize, x, y);
		nodes.decisionNodes.push_back(d);
		res.address = d;
		res.type = NodeType_Decision;
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
	nodes.startNode = nullptr;
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