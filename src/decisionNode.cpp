#include "decisionNode.h"

#include "raylib.h"
#include "cstring"

DecisionNode* NewDecisionNode(int padding, int fontSize, float x, float y) {
	DecisionNode* p = new DecisionNode;
	p->id = -1;
	char* temp = new char[9];
	strcpy(temp, "Decision");
	p->label = temp;
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
	p->inPin.ownerType = decision;

	p->outPinTrue.id = 0;
	p->outPinTrue.type = output;
	p->outPinTrue.x = 0.0f;
	p->outPinTrue.y = 0.0f;
	p->outPinTrue.radius = PIN_RADIUS;
	p->outPinTrue.owner = p;
	p->outPinTrue.ownerType = decision;

	p->outPinFalse.id = 0;
	p->outPinFalse.type = output;
	p->outPinFalse.x = 0.0f;
	p->outPinFalse.y = 0.0f;
	p->outPinFalse.radius = PIN_RADIUS;
	p->outPinFalse.owner = p;
	p->outPinFalse.ownerType = decision;

	p->toPinTrue = nullptr;
	p->toPinFalse = nullptr;

	SetDecisionNodePosition(p, x, y);
	SetDecisionNodeSize(p, padding, fontSize);

	return p;
}
void SetDecisionNodeSize(DecisionNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label, fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPinTrue.x = node->x;
	node->outPinTrue.y = node->y + node->height / 2.0f;

	node->outPinFalse.x = node->x + node->width;
	node->outPinFalse.y = node->y + node->height / 2.0f;
}
void SetDecisionNodePosition(DecisionNode* node, float x, float y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPinTrue.x = node->x;
	node->outPinTrue.y = node->y + node->height / 2.0f;

	node->outPinFalse.x = node->x + node->width;
	node->outPinFalse.y = node->y + node->height / 2.0f;
}
void DrawDecisionNode(DecisionNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, PURPLE);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, WHITE);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPinTrue.x, node->outPinTrue.y, node->outPinTrue.radius, GRAY);
	DrawCircle(node->outPinFalse.x, node->outPinFalse.y, node->outPinFalse.radius, GRAY);
	DrawLink(node->outPinTrue, node->toPinTrue);
	DrawLink(node->outPinFalse, node->toPinFalse);
}