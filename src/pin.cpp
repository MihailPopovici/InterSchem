#include "pin.h"
#include "nodeComponents.h"

#include "raylib.h"

void DrawLink(Pin& from, Pin* to) {
	if (to == nullptr) return;
	if (from.y <= to->y) {
		DrawLineEx({ from.x, from.y }, { from.x, (from.y + to->y) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ from.x, (from.y + to->y) / 2.0f }, { to->x, (from.y + to->y) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ to->x, (from.y + to->y) / 2.0f }, { to->x, to->y }, 2.0f, GRAY);

		DrawTriangle({ from.x, from.y }, { from.x - 4, from.y + 8 }, { from.x + 4, from.y + 8 }, GRAY);
		DrawTriangle({ to->x, to->y }, { to->x + 4, to->y - 8 }, { to->x - 4, to->y - 8 }, GRAY);
	}
	else {
		DrawLineEx({ from.x, from.y }, { (from.x + to->x) / 2.0f, from.y }, 2.0f, GRAY);
		DrawLineEx({ (from.x + to->x) / 2.0f, from.y }, { (from.x + to->x) / 2.0f, to->y }, 2.0f, GRAY);
		DrawLineEx({ (from.x + to->x) / 2.0f, to->y }, { to->x, to->y }, 2.0f, GRAY);

		DrawTriangle({ from.x, from.y }, { from.x + 8, from.y + 4 }, { from.x + 8, from.y - 4 }, GRAY);
		DrawTriangle({ to->x, to->y }, { to->x - 8, to->y - 4 }, { to->x - 8,to->y + 4 }, GRAY);
	}
}
void DrawGhostLink(AnyNodeType& node, float mx, float my) {
	float x = 0.0f, y = 0.0f;

	if (node.type == start) {
		StartNode* n = (StartNode*)node.address;
		x = n->outPin.x;
		y = n->outPin.y;
	}
	else if (node.type == read) {
		ReadNode* n = (ReadNode*)node.address;
		x = n->outPin.x;
		y = n->outPin.y;
	}
	else if (node.type == write) {
		WriteNode* n = (WriteNode*)node.address;
		x = n->outPin.x;
		y = n->outPin.y;
	}
	else if (node.type == assign) {
		AssignNode* n = (AssignNode*)node.address;
		x = n->outPin.x;
		y = n->outPin.y;
	} // TODO:
	/*else if (node.type == decision) { 
		DecisionNode* n = (DecisionNode*)node.address;
		x = n->outPin.x;
		y = n->outPin.y;
	}
	else {
		StopNode* n = (StopNode*)node.address;
		x = n->outPin.x;
		y = n->outPin.y;
	}*/

	if (y <= my) {
		DrawLineEx({ x, y }, { x, (y + my) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ x, (y + my) / 2.0f }, { mx, (y + my) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ mx, (y + my) / 2.0f }, { mx, my }, 2.0f, GRAY);

		DrawTriangle({ x, y }, { x - 4, y + 8 }, { x + 4, y + 8 }, GRAY);
		DrawTriangle({ mx, my }, { mx + 4, my - 8 }, { mx - 4, my - 8 }, GRAY);
	}
	else {
		DrawLineEx({ x, y }, { (x + mx) / 2.0f, y }, 2.0f, GRAY);
		DrawLineEx({ (x + mx) / 2.0f, y }, { (x + mx) / 2.0f, my }, 2.0f, GRAY);
		DrawLineEx({ (x + mx) / 2.0f, my }, { mx, my }, 2.0f, GRAY);

		DrawTriangle({ x, y }, { x + 8, y + 4 }, { x + 8, y - 4 }, GRAY);
		DrawTriangle({ mx, my }, { mx - 8, my - 4 }, { mx - 8,my + 4 }, GRAY);
	}
}
void DrawGhostLink(Pin* pin, float mx, float my) {
	float x = pin->x, y = pin->y;

	if (y <= my) {
		DrawLineEx({ x, y }, { x, (y + my) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ x, (y + my) / 2.0f }, { mx, (y + my) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ mx, (y + my) / 2.0f }, { mx, my }, 2.0f, GRAY);

		DrawTriangle({ x, y }, { x - 4, y + 8 }, { x + 4, y + 8 }, GRAY);
		DrawTriangle({ mx, my }, { mx + 4, my - 8 }, { mx - 4, my - 8 }, GRAY);
	}
	else {
		DrawLineEx({ x, y }, { (x + mx) / 2.0f, y }, 2.0f, GRAY);
		DrawLineEx({ (x + mx) / 2.0f, y }, { (x + mx) / 2.0f, my }, 2.0f, GRAY);
		DrawLineEx({ (x + mx) / 2.0f, my }, { mx, my }, 2.0f, GRAY);

		DrawTriangle({ x, y }, { x + 8, y + 4 }, { x + 8, y - 4 }, GRAY);
		DrawTriangle({ mx, my }, { mx - 8, my - 4 }, { mx - 8,my + 4 }, GRAY);
	}
}
void NewLink(Pin*& first, Pin& second) {
	first = &second;
}