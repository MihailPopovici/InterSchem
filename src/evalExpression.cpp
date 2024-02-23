#include "evalExpression.h"


bool isFunction(std::string s) {
    if (s == "abs" || s == "sin" || s == "cos" || s == "log" || s == "sqrt") return 1;
    return 0;
}
bool isNumber(std::string s) {
    if (s.size() == 1 && !(s[0] >= '0' && s[0] <= '9')) return false;
    //can contain only digits, dots and minus sign on the first index
    if (!(s[0] >= '0' && s[0] <= '9') && s[0] != '-') return false;
    for (int i = 1; i < s.size(); i++) {
        if (!(s[i] >= '0' && s[i] <= '9') && s[i] != '.') return false;
    }
    return true;
}
bool isOperator(std::string s) {
    return (s == "+" || s == "-" || s == "*" || s == "/" || s == "^" || s=="%");
}
bool isParenthesis(std::string s) {
    return (s == "(" || s == ")");
}
bool isVariable(std::string s) {
    return (!isNumber(s) && !isOperator(s) && !isParenthesis(s) && !isFunction(s));
}
bool correctVariableName(std::string s) {
    if (s[0] >= '0' && s[0] <= '9') return false;
    for (int i = 0; i < s.size(); i++) {
        if (!(s[i] >= 'a' && s[i] <= 'z') && s[i] != '_' && !(s[i] >= '0' && s[i] <= '9'))
            return false;
    }
    return true;
}
int priority(std::string s) {
    if (s == "+" || s == "-") return 1;
    if (s == "*" || s == "/" || s=="%") return 2;
    if (s == "^") return 3;
    if (isFunction(s)) return 4;
    return -1;
}
float applyOp(float val1, float val2, std::string op) {
    if (op == "+") return val1 + val2;
    if (op == "-") return val1 - val2;
    if (op == "*") return val1 * val2;
    if (op == "/" && val2 != 0) return val1 / val2;
    if (op == "^") return pow(val1, val2);
    if (op == "%" && val2 != 0) return (int)val1 % (int)val2;
    return -1;
}
float applyFunction(float val, std::string func) {
    if (func == "abs") return abs(val);
    if (func == "sin") return sin(val);
    if (func == "cos") return cos(val);
    if (func == "log") return log(val);
    if (func == "sqrt") return sqrt(val);
    return -1;
}
int nrOfPharentesis(std::vector<std::string>& tokens) {
    int opened = 0, closed = 0;
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "(")opened++;
        if (tokens[i] == ")") closed++;
    }
    return opened - closed;
}
int nrOfDots(std::string s) {
    int cnt = 0;
    for (int i = 0; i < s.size(); i++)
        if (s[i] == '.') cnt++;
    return cnt;
}
void SplitIntoTokens(std::string s, std::vector<std::string>& tokens) {
    std::string operators = "()+-*/^%";
    std::string token = "";
    for (int i = 0; i < s.size(); i++) {
        if (operators.find(s[i]) != std::string::npos) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back({ s[i] });
        }
        else {
            token += s[i];
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
}
void CheckForUnaryMinus(std::vector<std::string>& tokens) {
    //if there is a minus on the first position
    if (tokens[0] == "-") {
        tokens[0] = "-1";
        tokens.insert(tokens.begin() + 1, "*");
    }
    //if there is a minus inside Parenthesis
    for (int i = 1; i < tokens.size(); i++) {
        if (tokens[i] == "-" && tokens[i - 1] == "(") {
            tokens[i] = "-1";
            tokens.insert(tokens.begin() + i + 1, "*");
        }
    }
}

bool CheckCorrectitude(std::vector<std::string> tokens) {
    if (nrOfPharentesis(tokens) != 0) return false;
    for (int i = 0; i < tokens.size() - 1; i++) {
        if (isVariable(tokens[i]) && correctVariableName(tokens[i]) == false) return false;
        if (isNumber(tokens[i]) && nrOfDots(tokens[i]) > 1) return false;
        if ((isNumber(tokens[i]) || isVariable(tokens[i])) && !(isOperator(tokens[i + 1]) || tokens[i + 1] == ")")) return false;
        if (isOperator(tokens[i]) && !(isNumber(tokens[i + 1]) || isVariable(tokens[i + 1]) || isFunction(tokens[i + 1]) || tokens[i + 1] == "(")) return false;
        if (isFunction(tokens[i]) && tokens[i + 1] != "(")return false;
        if (tokens[i] == "(" && !(tokens[i + 1] == "-" || isNumber(tokens[i + 1]) || isVariable(tokens[i + 1]) || isFunction(tokens[i + 1]) || tokens[i + 1] == ")")) return false;
        if (tokens[i] == ")" && !(isOperator(tokens[i + 1]) || tokens[i + 1] == ")")) return false;
    }
    if (isVariable(tokens[tokens.size() - 1]) && correctVariableName(tokens[tokens.size() - 1]) == false) return false;
    if (isNumber(tokens[tokens.size() - 1]) && nrOfDots(tokens[tokens.size() - 1]) > 1) return false;
    return true;
}
float evaluate(std::string expression, Dictionary* dict, int& err) {
    //declare our stacks
    std::stack <float> values;
    std::stack <std::string> operators;
    std::vector<std::string> tokens;
    SplitIntoTokens(expression, tokens);
    bool ok = CheckCorrectitude(tokens);
    if (ok == false) {
        err = -1;
        return -1;
    }
    //check for unary operators
    CheckForUnaryMinus(tokens);

    for (int i = 0; i < tokens.size(); i++) {
        //if we have a number
        if (isNumber(tokens[i])) {
            float number = stof(tokens[i]);
            values.push(number);
        }
        //if we have a variable
        else if (isVariable(tokens[i])) {
            float number = GetValueFromDictionary(dict, tokens[i]);
            values.push(number);
        }
        //if we have an opening bracket : (
        else  if (tokens[i] == "(")
            operators.push(tokens[i]);
        //if we have a closing bracket : )
        else if (tokens[i] == ")") {
            while (!operators.empty() && operators.top() != "(") {
                if (isFunction(operators.top())) {
                    float val = values.top();
                    values.pop();
                    std::string op = operators.top();
                    operators.pop();

                    values.push(applyFunction(val, op));
                }
                else {
                    float val1 = values.top();
                    values.pop();
                    float val2 = values.top();
                    values.pop();
                    std::string op = operators.top();
                    operators.pop();

                    values.push(applyOp(val2, val1, op));
                }
            }
            operators.pop();
        }
        //if we have an operator
        else {
            while (!operators.empty() && priority(operators.top()) >= priority(tokens[i])) {
                if (isFunction(operators.top())) {
                    float val = values.top();
                    values.pop();
                    std::string op = operators.top();
                    operators.pop();

                    values.push(applyFunction(val, op));
                }
                else {
                    float val1 = values.top();
                    values.pop();
                    float val2 = values.top();
                    values.pop();
                    std::string op = operators.top();
                    operators.pop();

                    values.push(applyOp(val2, val1, op));
                }
            }
            operators.push(tokens[i]);
        }
    }
    //now, the entire expression has been parsed, so we apply the remaining operators to the remaining values
    while (!operators.empty()) {
        if (isFunction(operators.top())) {
            float val = values.top();
            values.pop();
            std::string op = operators.top();
            operators.pop();

            values.push(applyFunction(val, op));
        }
        else {
            float val1 = values.top();
            values.pop();
            float val2 = values.top();
            values.pop();
            std::string op = operators.top();
            operators.pop();
			values.push(applyOp(val2, val1, op));
        }
    }
    return values.top();
}

//Logical expressions
bool isComparisonOperator(std::string s) {
    return (s == "<" || s == ">" || s == "!" || s == "=");
}
bool isLogicalOperator(std::string s) {
    return (s == "&&" || s == "||");
}
bool applyLogicalOperator(float value1, std::string op, float value2) {
    if (op == "==") return value1 == value2;
    if (op == "!=") return value1 != value2;
    if (op == "<=") return value1 <= value2;
    if (op == ">=") return value1 >= value2;
    if (op == ">") return value1 > value2;
    if (op == "<") return value1 < value2;
    return 0;
}
bool applyOperator(bool value1, std::string op, bool value2) {
    if (op == "&&") return value1 && value2;
    if (op == "||") return value1 || value2;
    return 0;
}
int logicalPriority(std::string s) {
    if (s == "||") return 1;
    if (s == "&&")return 2;
    return -1;
}
bool evaluateComparison(std::string s,Dictionary* dict, int& errorCode) {
    std::string expression1, op, expression2;
    int i = 0;
    while (isComparisonOperator({ s[i] }) == false) {
        expression1 += s[i];
        i++;
    }
    op += s[i];
    i++;
    if (isComparisonOperator({ s[i] }) == true)op += s[i], i++;
    while (i < s.size()) {
        expression2 += s[i];
        i++;
    }
    float value1, value2;
    value1 = evaluate(expression1,dict, errorCode);
    value2 = evaluate(expression2,dict, errorCode);
    if (errorCode == -1) {
        return 0;
    }
    return applyLogicalOperator(value1, op, value2);
}
void checkForZeroComparison(std::vector<std::string>& tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        if (isLogicalOperator(tokens[i]) == false) {
            //current token is a comparison
            bool ok = false;
            for (int j = 0; j < tokens[i].size(); j++)
                if (isComparisonOperator({ tokens[i][j] }) == true) ok = true;
            if (ok == false) {
                tokens[i] += "!=0";
            }
        }
    }
}
bool checkLogicalCorrectitude(std::vector<std::string>& tokens) {
    if (tokens.size() == 1 && isLogicalOperator(tokens[0])) return false;
    for (int i = 0; i < tokens.size() - 1; i++) {
        if (isLogicalOperator(tokens[i]) && isLogicalOperator(tokens[i + 1])) return false;
    }
    return true;
}
void SplitIntoLogicalTokens(std::string s, std::vector<std::string>& tokens, int& errorCode) {
    std::string operators = "&|";
    std::string token = "";
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != ' ') {
            if (operators.find(s[i]) != std::string::npos) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                token = { s[i] };
                if (s[i + 1] != s[i]) {
                    errorCode = -1;
                    return;
                }
                token += s[i + 1], i++;
                tokens.push_back(token);
                token.clear();
            }
            else {
                token += s[i];
            }
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
}
bool evaluateLogicalExpression(std::string s,Dictionary* dict, int& errorCode) {
    std::stack <std::string> operators;
    std::stack <bool> truthValues;

    std::vector<std::string> tokens;
    SplitIntoLogicalTokens(s, tokens, errorCode);
    if (errorCode == -1) return 0;
    bool ok = checkLogicalCorrectitude(tokens);
    if (ok == false) {
        errorCode = -2;
        return 0;
    }
    checkForZeroComparison(tokens);
    for (int i = 0; i < tokens.size(); i++) {
        //if we have an expression
        if (isLogicalOperator(tokens[i]) == false) {
            bool truth = evaluateComparison(tokens[i],dict, errorCode);
            truthValues.push(truth);
        }
        //if we have a logical operator
        else {
            while (!operators.empty() && logicalPriority(operators.top()) >= logicalPriority(tokens[i])) {
                std::string op = operators.top();
                operators.pop();
                bool value1 = truthValues.top();
                truthValues.pop();
                bool value2 = truthValues.top();
                truthValues.pop();
                truthValues.push(applyOperator(value1, op, value2));
            }
            operators.push(tokens[i]);
        }
    }
    while (!operators.empty()) {
        std::string op = operators.top();
        operators.pop();
        bool value1 = truthValues.top();
        truthValues.pop();
        bool value2 = truthValues.top();
        truthValues.pop();
        truthValues.push(applyOperator(value1, op, value2));
    }
    return truthValues.top();
}