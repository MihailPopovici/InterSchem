#pragma once

#include "nodeComponents.h"
#include "dictionary.h"
#include "multilinetext.h"
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>
#include <utility>

struct Button;

enum ExecutionState {
	notExecuting, // TODO: find a better name?
	processing,
	waitingForInput,
	done
};

void GetClickedNode(AnyNodeType& clickedNode, int mx, int my, NodeArrays& nodes);
void GetNextNodeInExecution(AnyNodeType& currentNode, ExecutionState& state, Dictionary* dict, MultiLineText* console);
void DrawSelectedNodeOptions(AnyNodeType& node, Button* del);
void DragNode(AnyNodeType& node, int mx, int my);
void GetClickedPin(Pin*& pin, NodeArrays& nodes);
int GetBestDistToPin(NodeArrays& nodes, int x, int y);
void EraseNode(NodeArrays& nodes, AnyNodeType node);
void EraseNodeLinks(NodeArrays& nodes, Pin* inPin);
void UpdateVariablesTable(NodeArrays& nodes, Dictionary* dict);
void EncodeScheme(MultiLineText* code, Dictionary* dict, void* startNode);