#include "assignNode.h"

#include "raylib.h"
#include <string>
#include "evalExpression.h"

AssignNode* NewAssignNode(int padding, int fontSize, float x, float y) {
	AssignNode* p = new AssignNode;
	p->id = -1;
	p->label = "Assign";
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = 0;
	p->inPin.type = input;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->inPin.owner = p;
	p->inPin.ownerType = assign;

	p->outPin.id = 0;
	p->outPin.type = output;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;
	p->outPin.owner = p;
	p->outPin.ownerType = assign;

	p->toPin = nullptr;

	SetAssignNodePosition(p, x, y);
	SetAssignNodeSize(p, padding, fontSize);

	return p;
}
void SetAssignNodeSize(AssignNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label.c_str(), fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void SetAssignNodePosition(AssignNode* node, float x, float y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void DrawAssignNode(AssignNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, ORANGE);
	DrawText(node->label.c_str(), node->x + node->padding, node->y + node->padding, node->fontSize, BLACK);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
	DrawLink(node->outPin, node->toPin);
	/*if (node->myVarValue != nullptr) {
		DrawText("unlinked", node->x + node->width + 5, node->y + 5, node->fontSize, WHITE);
	}*/

	// +----+
	// |    |
	// +----+

	// +------------------+
	// | a = sin(6) + 5*x |
	// +------------------+

	// +-------+
	// | a = 2 |
	// +-------+
}
void LinkAssignNodeVar(AssignNode* node, std::string* name, int* val) {
	node->myVarName = name;
	node->myVarValue = val;
	node->label = *name;
	node->width = MeasureText((*name).c_str(), node->fontSize) + 2 * node->padding;
	node->inPin.x = node->x + node->width / 2;
	node->outPin.x = node->x + node->width / 2;
}
void SetAssignNodeExpression(AssignNode* node, std::string expression) {
	node->expression = expression;
	node->label += " = " + expression;
	node->width = MeasureText(node->label.c_str(), node->fontSize) + 2 * node->padding;
	node->inPin.x = node->x + node->width / 2;
	node->outPin.x = node->x + node->width / 2;
}
void EvaluateAssignNode(AssignNode* node) {
	int err = 0;
	float result = evaluate(node->expression, err);
	if (err != 0) {
		// TODO: popup
	}
	else {
		*node->myVarValue = result;
	}
}