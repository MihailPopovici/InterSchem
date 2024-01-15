#pragma once

enum NodeType {
	NodeType_NoType,
	NodeType_Start,
	NodeType_Read,
	NodeType_Write,
	NodeType_Assign,
	NodeType_Decision,
	NodeType_Stop
};

struct AnyNodeType {
	void* address;
	NodeType type;
};