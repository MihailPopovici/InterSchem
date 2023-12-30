#pragma once

enum NodeType {
	noType,
	start,
	read,
	write,
	assign,
	decision,
	stop
};

//struct NodeInfo {
//	int id, index;
//	NodeType type;
//};

struct AnyNodeType {
	void* address;
	NodeType type;
};