#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>
#include <stack>
#include "ZContextVar.h"
#include "VarTypes.h"

enum ExprOperatorType {

    OpPlus, OpMinus, OpMultiply, OpDivide, OpLeftBrace, OpRightBrace,OpGreater,OpLess

};

enum ExprElementType {

    EInt,EFloat,EString,EOp,SubExpression,EVar,EStatement,EClassStatement

};


class ZStatementNode;
class ZClassStatementNode;
class Expression;

class ExpressionElement {

public:
    int mValInt = 0;
    float mValFloat = 0;
    std::string mValName[32];
    std::string mValString;
    void* mSubExpression;
    ExprOperatorType mOp; 
    ExprElementType mType;
    ZStatementNode* mStatement = nullptr;
    ZClassStatementNode* mClassStatement = nullptr;

};
int precedence(ExprOperatorType op);
int applyOpInt(int a, int b, ExprOperatorType op);
float applyOpFloat(float a, float b, ExprOperatorType op);
int evaluateInt(std::vector<ExpressionElement> mElements);
float evaluateFloat(std::vector<ExpressionElement> mElements);

/*
int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}
*/


class Expression
{
public:

    std::vector<ExpressionElement> mElements;
    ZContextVar* Evaluate();
    

};

class ZExpressionNode :
    public ZScriptNode
{
public:

    //void AddElement(ExpressionElement element);
    void SetExpression(Expression expr);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    Expression GetExpression();

private:

    Expression mExpression;

};

