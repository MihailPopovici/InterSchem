#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stack>

struct var {
    std::string name;
    float value;
};

bool isFunction(std::string s);
bool isNumber(std::string s);
bool isOperator(std::string s);
bool isParenthesis(std::string s);
bool isVariable(std::string s);
bool correctVariableName(std::string s);
int priority(std::string s);
float applyOp(float val1, float val2, std::string op);
float applyFunction(float val, std::string func);
int nrOfPharentesis(std::vector<std::string>& tokens);
int nrOfDots(std::string s);
void SplitIntoTokens(std::string s, std::vector<std::string>& tokens);
void CheckForUnaryMinus(std::vector<std::string>& tokens);
void AssignValues(std::vector<std::string>& tokens);
float GetVarValue(std::string s);
bool CheckCorrectitude(std::vector<std::string> tokens);
float evaluate(std::string expression, int& err);

//Logical expressions
bool isComparisonOperator(std::string s);
bool isLogicalOperator(std::string s);
bool applyLogicalOperator(float value1, std::string op, float value2);
bool applyOperator(bool value1, std::string op, bool value2);
int logicalPriority(std::string s);
bool evaluateComparison(std::string s, int& errorCode);
void checkForZeroComparison(std::vector<std::string>& tokens);
bool checkLogicalCorrectitude(std::vector<std::string>& tokens);
void SplitIntoLogicalTokens(std::string s, std::vector<std::string>& tokens, int& errorCode);
bool evaluateLogicalExpression(std::string s, int& errorCode);