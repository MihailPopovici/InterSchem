#pragma once

enum NodeType {
	start,
	read,
	write,
	assign,
	decision,
	stop
};
struct NodeInfo {
	int id, index;
	NodeType type;
};