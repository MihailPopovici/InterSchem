#include "fileFunc.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>

std::vector<std::string> GetSchemeFileNames() {
	namespace fs = std::filesystem;
	std::vector<std::string> vec;
	for (auto& file : fs::directory_iterator("schemes/")) {
		if (file.path().extension() == ".interschem") {
			std::string fullName = file.path().filename().string();
			vec.push_back(fullName.substr(0, fullName.find_first_of('.')));
		}
	}
	return vec;
}
bool SaveSchemeToFile(NodeArrays& nodes, std::string path) {
	std::ofstream file(path, std::ofstream::binary);
	if (file.fail()) {
		return false;
	}
	file.clear();

	file.write((char*)(&nodes.startNode), sizeof(StartNode*));
	if (nodes.startNode != nullptr) {
		file.write((char*)(nodes.startNode), sizeof(StartNode) - sizeof(Pin*));
		if (nodes.startNode->toPin != nullptr) {
			file.write((char*)(&nodes.startNode->toPin->ownerPtr), sizeof(void*));
		}
		else {
			file.write((char*)(&nodes.startNode->toPin), sizeof(void*));
		}
	}

	size_t n = nodes.readNodes.size();
	file.write((char*)(&n), sizeof(size_t));
	for (ReadNode*& p : nodes.readNodes) {
		file.write((char*)(&p), sizeof(ReadNode*));
		file.write((char*)(p), sizeof(ReadNode) - sizeof(SingleLineText*) - sizeof(Pin*));
		if (p->toPin != nullptr) {
			file.write((char*)(&p->toPin->ownerPtr), sizeof(void*));
		}
		else {
			file.write((char*)(&p->toPin), sizeof(void*));
		}
		file.write((char*)(p->myVarName), sizeof(SingleLineText) - sizeof(std::string));
		size_t len = p->myVarName->str.size();
		file.write((char*)(&len), sizeof(size_t));
		if (len > 0) {
			file.write(p->myVarName->str.c_str(), len);
		}
	}

	n = nodes.writeNodes.size();
	file.write((char*)(&n), sizeof(size_t));
	for (WriteNode*& p : nodes.writeNodes) {
		file.write((char*)(&p), sizeof(WriteNode*));
		file.write((char*)(p), sizeof(WriteNode) - sizeof(SingleLineText*) - sizeof(Pin*));
		if (p->toPin != nullptr) {
			file.write((char*)(&p->toPin->ownerPtr), sizeof(void*));
		}
		else {
			file.write((char*)(&p->toPin), sizeof(void*));
		}
		file.write((char*)(p->expression), sizeof(SingleLineText) - sizeof(std::string));
		size_t len = p->expression->str.size();
		file.write((char*)(&len), sizeof(size_t));
		if (len > 0) {
			file.write(p->expression->str.c_str(), len);
		}
	}

	n = nodes.assignNodes.size();
	file.write((char*)(&n), sizeof(size_t));
	for (AssignNode*& p : nodes.assignNodes) {
		file.write((char*)(&p), sizeof(AssignNode*));
		file.write((char*)(p), sizeof(AssignNode) - 2 * sizeof(SingleLineText*) - sizeof(Pin*));
		if (p->toPin != nullptr) {
			file.write((char*)(&p->toPin->ownerPtr), sizeof(void*));
		}
		else {
			file.write((char*)(&p->toPin), sizeof(void*));
		}
		file.write((char*)(p->myVarName), sizeof(SingleLineText) - sizeof(std::string));
		size_t len = p->myVarName->str.size();
		file.write((char*)(&len), sizeof(size_t));
		if (len > 0) {
			file.write(p->myVarName->str.c_str(), len);
		}
		file.write((char*)(p->expression), sizeof(SingleLineText) - sizeof(std::string));
		len = p->expression->str.size();
		file.write((char*)(&len), sizeof(size_t));
		if (len > 0) {
			file.write(p->expression->str.c_str(), len);
		}
	}

	n = nodes.decisionNodes.size();
	file.write((char*)(&n), sizeof(size_t));
	for (DecisionNode*& p : nodes.decisionNodes) {
		file.write((char*)(&p), sizeof(DecisionNode*));
		file.write((char*)(p), sizeof(DecisionNode) - sizeof(SingleLineText*) - 2 * sizeof(Pin*));
		if (p->toPinTrue != nullptr) {
			file.write((char*)(&p->toPinTrue->ownerPtr), sizeof(void*));
		}
		else {
			file.write((char*)(&p->toPinTrue), sizeof(void*));
		}
		if (p->toPinFalse != nullptr) {
			file.write((char*)(&p->toPinFalse->ownerPtr), sizeof(void*));
		}
		else {
			file.write((char*)(&p->toPinFalse), sizeof(void*));
		}
		file.write((char*)(p->expression), sizeof(SingleLineText) - sizeof(std::string));
		size_t len = p->expression->str.size();
		file.write((char*)(&len), sizeof(size_t));
		if (len > 0) {
			file.write(p->expression->str.c_str(), len);
		}
	}

	n = nodes.stopNodes.size();
	file.write((char*)(&n), sizeof(size_t));
	for (StopNode*& p : nodes.stopNodes) {
		file.write((char*)(&p), sizeof(StopNode*));
		file.write((char*)(p), sizeof(StopNode));
	}
	return true;
}
bool LoadSchemeFromFile(NodeArrays& nodes, std::string path) {
	std::ifstream file(path);
	if (file.fail()) {
		return false;
	}
	std::unordered_map<void*, AnyNodeType> oldToNew; // vechile adrese ale nodurilor -> noile adrese ale nodurilor
	std::unordered_map<void*, Pin*> nodesInPin; // vechile adrese ale nodurilor -> adresa inPin-ului al noului nod corespunzator
	std::vector<Link> links;
	
	void* node = nullptr;
	file.read((char*)(&node), sizeof(StartNode*));
	if (node != nullptr) {
		auto newNode = NewNode(nodes, start, 5, 20, 100, 100);
		oldToNew[node] = newNode;
		file.read((char*)(newNode.address), sizeof(StartNode) - sizeof(Pin*));
		StartNode* p = (StartNode*)newNode.address;
		p->outPin.ownerPtr = newNode.address;
		void* toNode = nullptr;
		file.read((char*)(&toNode), sizeof(Pin*));
		if (toNode != nullptr) {
			links.push_back({ node, toNode, LinkType_Normal });
		}
	}

	size_t n = 0;
	file.read((char*)(&n), sizeof(size_t));
	for (size_t i = 1; i <= n; i++) {
		file.read((char*)(&node), sizeof(ReadNode*));
		auto newNode = NewNode(nodes, read, 5, 20, 100, 100);
		ReadNode* p = (ReadNode*)newNode.address;
		oldToNew[node] = newNode;
		nodesInPin[node] = &p->inPin;
		file.read((char*)(newNode.address), sizeof(ReadNode) - sizeof(SingleLineText*) - sizeof(Pin*));
		p->inPin.ownerPtr = newNode.address;
		p->outPin.ownerPtr = newNode.address;
		void* toNode = nullptr;
		file.read((char*)(&toNode), sizeof(Pin*));
		if (toNode != nullptr) {
			links.push_back({ node, toNode, LinkType_Normal });
		}
		file.read((char*)(p->myVarName), sizeof(SingleLineText) - sizeof(std::string));
		size_t len = 0;
		file.read((char*)(&len), sizeof(size_t));
		if (len > 0) {
			char* str = new char[len + 1];
			str[len] = 0;
			file.read(str, len);
			p->myVarName->str = str;
			delete[] str;
		}
	}

	file.read((char*)(&n), sizeof(size_t));
	for (size_t i = 1; i <= n; i++) {
		file.read((char*)(&node), sizeof(WriteNode*));
		auto newNode = NewNode(nodes, write, 5, 20, 100, 100);
		WriteNode* p = (WriteNode*)newNode.address;
		oldToNew[node] = newNode;
		nodesInPin[node] = &p->inPin;
		file.read((char*)(newNode.address), sizeof(WriteNode) - sizeof(SingleLineText*) - sizeof(Pin*));
		p->inPin.ownerPtr = newNode.address;
		p->outPin.ownerPtr = newNode.address;
		void* toNode = nullptr;
		file.read((char*)(&toNode), sizeof(Pin*));
		if (toNode != nullptr) {
			links.push_back({ node, toNode, LinkType_Normal });
		}
		file.read((char*)(p->expression), sizeof(SingleLineText) - sizeof(std::string));
		size_t len = 0;
		file.read((char*)(&len), sizeof(size_t));
		if (len > 0) {
			char* str = new char[len + 1];
			str[len] = 0;
			file.read(str, len);
			p->expression->str = str;
			delete[] str;
		}
	}

	file.read((char*)(&n), sizeof(size_t));
	for (size_t i = 1; i <= n; i++) {
		file.read((char*)(&node), sizeof(AssignNode*));
		auto newNode = NewNode(nodes, assign, 5, 20, 100, 100);
		AssignNode* p = (AssignNode*)newNode.address;
		oldToNew[node] = newNode;
		nodesInPin[node] = &p->inPin;
		file.read((char*)(newNode.address), sizeof(AssignNode) - 2 * sizeof(SingleLineText*) - sizeof(Pin*));
		p->inPin.ownerPtr = newNode.address;
		p->outPin.ownerPtr = newNode.address;
		void* toNode = nullptr;
		file.read((char*)(&toNode), sizeof(Pin*));
		if (toNode != nullptr) {
			links.push_back({ node, toNode, LinkType_Normal });
		}
		file.read((char*)(p->myVarName), sizeof(SingleLineText) - sizeof(std::string));
		size_t len = 0;
		file.read((char*)(&len), sizeof(size_t));
		if (len > 0) {
			char* str = new char[len + 1];
			str[len] = 0;
			file.read(str, len);
			p->myVarName->str = str;
			delete[] str;
		}
		file.read((char*)(p->expression), sizeof(SingleLineText) - sizeof(std::string));
		file.read((char*)(&len), sizeof(size_t));
		if (len > 0) {
			char* str = new char[len + 1];
			str[len] = 0;
			file.read(str, len);
			p->expression->str = str;
			delete[] str;
		}
	}

	file.read((char*)(&n), sizeof(size_t));
	for (size_t i = 1; i <= n; i++) {
		file.read((char*)(&node), sizeof(DecisionNode*));
		auto newNode = NewNode(nodes, decision, 5, 20, 100, 100);
		DecisionNode* p = (DecisionNode*)newNode.address;
		oldToNew[node] = newNode;
		nodesInPin[node] = &p->inPin;
		file.read((char*)(newNode.address), sizeof(DecisionNode) - sizeof(SingleLineText*) - 2 * sizeof(Pin*));
		p->inPin.ownerPtr = newNode.address;
		p->outPinTrue.ownerPtr = newNode.address;
		p->outPinFalse.ownerPtr = newNode.address;
		void* toNode = nullptr;
		file.read((char*)(&toNode), sizeof(Pin*));
		if (toNode != nullptr) {
			links.push_back({ node, toNode, LinkType_True });
		}
		file.read((char*)(&toNode), sizeof(Pin*));
		if (toNode != nullptr) {
			links.push_back({ node, toNode, LinkType_False });
		}
		file.read((char*)(p->expression), sizeof(SingleLineText) - sizeof(std::string));
		size_t len = 0;
		file.read((char*)(&len), sizeof(size_t));
		if (len > 0) {
			char* str = new char[len + 1];
			str[len] = 0;
			file.read(str, len);
			p->expression->str = str;
			delete[] str;
		}
	}

	file.read((char*)(&n), sizeof(size_t));
	for (size_t i = 1; i <= n; i++) {
		file.read((char*)(&node), sizeof(StopNode*));
		auto newNode = NewNode(nodes, stop, 5, 20, 100, 100);
		StopNode* p = (StopNode*)newNode.address;
		oldToNew[node] = newNode;
		nodesInPin[node] = &p->inPin;
		file.read((char*)(newNode.address), sizeof(StopNode));
		p->inPin.ownerPtr = newNode.address;
	}

	for (auto& link : links) {
		AnyNodeType* from = &oldToNew[link.first];
		AnyNodeType* to = &oldToNew[link.second];
		Pin* toPin = nodesInPin[link.second];
		if (from->type == start) {
			NewLink(((StartNode*)from->address)->toPin, *toPin);
		}
		else if (from->type == read) {
			NewLink(((ReadNode*)from->address)->toPin, *toPin);
		}
		else if (from->type == write) {
			NewLink(((WriteNode*)from->address)->toPin, *toPin);
		}
		else if (from->type == assign) {
			NewLink(((AssignNode*)from->address)->toPin, *toPin);
		}
		else {
			if (link.type == LinkType_True) {
				NewLink(((DecisionNode*)from->address)->toPinTrue, *toPin);
			}
			else {
				NewLink(((DecisionNode*)from->address)->toPinFalse, *toPin);
			}
		}
	}

	return true;
}