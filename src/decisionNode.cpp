#include "decisionNode.h"

#include "raylib.h"
#include "string"

DecisionNode* NewDecisionNode(int padding, int fontSize, int x, int y) {
	DecisionNode* p = new DecisionNode;
	p->fontSize = 0;
	p->padding = 0;

	p->expression = NewSingleLineText();
	SetSingleLineTextColors(p->expression, { 100, 100, 100, 70 }, WHITE);
	SetSingleLineTextPadding(p->expression, 5);
	SetSingleLineTextFontSize(p->expression, 20);
	SetSingleLineTextPosition(p->expression, 0, 0);
	p->expression->visible = true;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.type = input;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->inPin.ownerPtr = p;
	p->inPin.ownerType = decision;

	p->outPinTrue.type = output;
	p->outPinTrue.x = 0.0f;
	p->outPinTrue.y = 0.0f;
	p->outPinTrue.radius = PIN_RADIUS;
	p->outPinTrue.ownerPtr = p;
	p->outPinTrue.ownerType = decision;

	p->outPinFalse.type = output;
	p->outPinFalse.x = 0.0f;
	p->outPinFalse.y = 0.0f;
	p->outPinFalse.radius = PIN_RADIUS;
	p->outPinFalse.ownerPtr = p;
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
	node->width = MeasureText(node->expression->str.c_str(), fontSize) + 2 * padding + 2*5;
	node->height = fontSize + 2 * padding + 2*5;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPinTrue.x = node->x;
	node->outPinTrue.y = node->y + node->height / 2.0f;

	node->outPinFalse.x = node->x + node->width;
	node->outPinFalse.y = node->y + node->height / 2.0f;
	SetSingleLineTextPosition(node->expression, node->x + node->padding, node->y + node->padding);
}
void SetDecisionNodePosition(DecisionNode* node, int x, int y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPinTrue.x = node->x;
	node->outPinTrue.y = node->y + node->height / 2.0f;

	node->outPinFalse.x = node->x + node->width;
	node->outPinFalse.y = node->y + node->height / 2.0f;
	SetSingleLineTextPosition(node->expression, node->x + node->padding, node->y + node->padding);
}
void DrawDecisionNode(DecisionNode* node) {
	Vector2 v1;
	v1.x = node->x + node->width / 2;
	v1.y = node->y;
	Vector2 v2;
	v2.x = node->x + node->width / 2;
	v2.y = node->y + node->height;
	Vector2 v3;
	v3.x = node->x;
	v3.y = node->y + node->height / 2;
	DrawTriangle(v3, v2, v1, PURPLE);
	v3 = v2;
	v2.x = node->x + node->width;
	v2.y = node->y + node->height / 2;
	DrawTriangle(v3, v2, v1, PURPLE);
	DrawSingleLineText(node->expression);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPinTrue.x, node->outPinTrue.y, node->outPinTrue.radius, GRAY);
	DrawCircle(node->outPinFalse.x, node->outPinFalse.y, node->outPinFalse.radius, GRAY);
	DrawLink(node->outPinTrue, node->toPinTrue);
	DrawLink(node->outPinFalse, node->toPinFalse);
}

int EvaluateDecisionNode(DecisionNode* node, Dictionary* dict) {
	int err = 0;
	bool truthValue = evaluateLogicalExpression(node->expression->str,dict, err);
	if (err != 0) {
		// TODO: popup
		return 0;
	}
	else {
		if (truthValue) return 1;
		else return 2;
	}
}

void ResizeDecisionNode(DecisionNode* node) {
	int expressionWidth = MeasureText(node->expression->str.c_str(), node->fontSize);
	node->width = 2 * node->padding + expressionWidth + 2 * 5;
	SetSingleLineTextPosition(node->expression, node->x  + node->padding, node->y + node->padding);

	node->inPin.x = node->x + node->width / 2.0f;

	node->outPinTrue.x = node->x;
	node->outPinFalse.x = node->x + node->width;
}
void GetInputDecisionNode(DecisionNode* node) {
	//std::cout << node->myVarName << " " << node->expression << "\n";
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		node->expression->focused = IsSingleLineTextClicked(node->expression);
	}
	if (node->expression->focused) {
		char c = GetCharPressed();
		if (c != 0) {
			InsertCharSingleLineText(node->expression, c);
			ResizeDecisionNode(node);
		}
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
		if (node->expression->focused) {
			EraseCharSingleLineText(node->expression);
		}
		ResizeDecisionNode(node);
	}
}