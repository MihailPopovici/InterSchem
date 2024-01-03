#pragma once

#include "nodeComponents.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "pin.h";

void SaveSchemeToFile(NodeArrays& nodes) {
	std::ofstream file("data1.txt");
	if (file.fail()) {
		return;
	}

	file << nodes.startNode << " ";
	if (nodes.startNode != nullptr) {
		file << nodes.startNode->x << " " << nodes.startNode->y << " ";
		if (nodes.startNode->toPin != nullptr) {
			file << nodes.startNode->toPin->owner;
		}
		else {
			file << 0;
		}
	}
	file << "\n";

	file << nodes.readNodes.size() << "\n";
	for (auto& p : nodes.readNodes) {
		file << p << " " << p->x << " " << p->y << " ";
		if (p->toPin != nullptr) {
			file << p->toPin->owner;
		}
		else {
			file << 0;
		}
		file << "\n";
	}

	file << nodes.writeNodes.size() << "\n";
	for (auto& p : nodes.writeNodes) {
		file << p << " " << p->x << " " << p->y << " ";
		if (p->toPin != nullptr) {
			file << p->toPin->owner;
		}
		else {
			file << 0;
		}
		file << "\n";
	}

	file.close();
}

NodeArrays LoadSchemeFromFile() {
	std::ifstream file("data1.txt");
	if (file.fail()) {
		return {};
	}
	std::unordered_map<void*, AnyNodeType> oldToNew; // vechile adrese ale nodurilor -> noile adrese ale nodurilor
	std::unordered_map<void*, Pin*> nodesInPin; // vechile adrese ale nodurilor -> adresa inPin-ului al noului nod corespunzator
	std::unordered_map<AnyNodeType*, void*> links; // noile adrese ale nodurilor -> legatura (dintre vechiul nod) cu vechiul nod corespunzator
	NodeArrays nodes;
	void* node = nullptr;
	file >> node;
	if (node != nullptr) {
		auto newNode = NewNode(nodes, start, 5, 20, 100, 100);
		oldToNew[node] = newNode;
		int x, y;
		file >> x >> y;
		SetStartNodePosition((StartNode*)newNode.address, x, y);
		void* toNode = nullptr;
		file >> toNode;
		if (toNode != nullptr) {
			links[&newNode] = toNode;
		}
	}
	size_t n = 0;
	file >> n;
	for (size_t i = 1; i <= n; i++) {
		file >> node;
		auto newNode = NewNode(nodes, read, 5, 20, 100, 100);
		oldToNew[node] = newNode;
		nodesInPin[node] = &((ReadNode*)newNode.address)->inPin;
		int x, y;
		file >> x >> y;
		SetReadNodePosition((ReadNode*)newNode.address, x, y);
		void* toNode = nullptr;
		file >> toNode;
		if (toNode != nullptr) {
			links[&newNode] = toNode;
		}
	}
	file >> n;
	for (size_t i = 1; i <= n; i++) {
		file >> node;
		auto newNode = NewNode(nodes, write, 5, 20, 100, 100);
		oldToNew[node] = newNode;
		nodesInPin[node] = &((WriteNode*)newNode.address)->inPin;
		int x, y;
		file >> x >> y;
		SetWriteNodePosition((WriteNode*)newNode.address, x, y);
		void* toNode = nullptr;
		file >> toNode;
		if (toNode != nullptr) {
			links[&newNode] = toNode;
		}
	}
	for (auto& [from, to] : links) {
		if (from->type == start) {
			NewLink(((StartNode*)from->address)->toPin, *nodesInPin[to]);
		}
		else if (from->type == read) {
			NewLink(((ReadNode*)from->address)->toPin, *nodesInPin[to]);
		}
		else if (from->type == write) {
			NewLink(((WriteNode*)from->address)->toPin, *nodesInPin[to]);
		}
	}

	file.close();
	return nodes;
}