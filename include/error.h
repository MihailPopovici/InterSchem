#pragma once
enum ErrorState {
	ErrorState_notErroring,
	ErrorState_incorrectMathExpression,
	ErrorState_incorrectLogicalExpression,
	ErrorState_incompleteScheme,
	ErrorState_incorrectVarName,
	ErrorState_emptyNode
};