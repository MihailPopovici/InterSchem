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

extern NodeArrays nodes;

AnyNodeType NewNode(NodeArrays& nodes, NodeType type, int padding, int fontSize, float x, float y);
void DrawNodes(NodeArrays& nodes);
void CleanupNodes(NodeArrays& nodes);