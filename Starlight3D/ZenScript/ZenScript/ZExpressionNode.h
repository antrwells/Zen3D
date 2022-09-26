#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>

enum ExprOperatorType {

    OpPlus,OpMinus,OpMultiply,OpDivide

};

enum ExprElementType {

    EInt,EFloat,EString,EOp,SubExpression

};


class Expression;

class ExpressionElement {

public:
    int mValInt = 0;
    float mValFloat = 0;
    std::string mValName;
    void* mSubExpression;
    ExprOperatorType mOp; 
    ExprElementType mType;

};

class Expression
{
public:

    std::vector<ExpressionElement> mElements;

};

class ZExpressionNode :
    public ZScriptNode
{
public:

    //void AddElement(ExpressionElement element);
    void SetExpression(Expression expr);

private:

    Expression mExpression;

};

