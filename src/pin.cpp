#include "pin.h"
#include "nodeComponents.h"

#include "raylib.h"

void DrawLink(Pin& from, Pin* to) {
	if (to == nullptr) return;
	float fromX = (float)from.x, fromY = (float)from.y, toX = (float)to->x, toY = (float)to->y;
	if (fromY <= toY) {
		DrawLineEx({ fromX, fromY }, { fromX, (fromY + toY) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ fromX, (fromY + toY) / 2.0f }, { toX, (fromY + toY) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ toX, (fromY + toY) / 2.0f }, { toX, toY }, 2.0f, GRAY);

		DrawTriangle({ fromX, fromY }, { fromX - 4, fromY + 8 }, { fromX + 4, fromY + 8 }, GRAY);
		DrawTriangle({ toX, toY }, { toX + 4, toY - 8 }, { toX - 4, toY - 8 }, GRAY);
	}
	else {
		DrawLineEx({ fromX, fromY }, { (fromX + toX) / 2.0f, fromY }, 2.0f, GRAY);
		DrawLineEx({ (fromX + toX) / 2.0f, fromY }, { (fromX + toX) / 2.0f, toY }, 2.0f, GRAY);
		DrawLineEx({ (fromX + toX) / 2.0f, toY }, { toX, toY }, 2.0f, GRAY);

		DrawTriangle({ fromX, fromY }, { fromX + 8, fromY + 4 }, { fromX + 8, fromY - 4 }, GRAY);
		DrawTriangle({ toX, toY }, { toX - 8, toY - 4 }, { toX - 8,toY + 4 }, GRAY);
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