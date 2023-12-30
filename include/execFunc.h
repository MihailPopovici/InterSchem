#pragma once

#include "nodeComponents.h"
//#include "uiComponents.h"

struct Button;

enum ExecutionState {
	notExecuting, // TODO: find a better name?
	processing,
	waitingForInput,
	done
};

void GetClickedNode(AnyNodeType& clickedNode, int mx, int my, NodeArrays& nodes);
void GetNextNodeInExecution(AnyNodeType& currentNode, ExecutionState& state);
void DrawSelectedNodeOptions(AnyNodeType& node, Button* del, Button* edit, Button* linkVar);
void DragNode(AnyNodeType& node, int mx, int my);
void GetClickedPin(Pin*& pin, int mx, int my, NodeArrays& nodes);
void EraseNode(NodeArrays& nodes, AnyNodeType node);
void EraseNodeLinks(NodeArrays& nodes, Pin* inPin);