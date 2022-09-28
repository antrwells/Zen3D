#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>
#include <stack>
#include "ZContextVar.h"
#include "VarTypes.h"

enum ExprOperatorType {

    OpPlus, OpMinus, OpMultiply, OpDivide, OpLeftBrace, OpRightBrace

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
    ZContextVar* Evaluate()
    {

        if (mElements[1].mType == EFloat)
        {
            ZContextVar* result = new ZContextVar("expr result", VarType::VarFloat);

            result->SetFloat(evaluateFloat(mElements));
            //        result->SetInt(101);
            int aa = 5;


            return result;
        }
        else if (mElements[1].mType == EInt)
        {
            ZContextVar* result = new ZContextVar("expr result", VarType::VarInt);

            result->SetInt(evaluateInt(mElements));
            //        result->SetInt(101);
            int aa = 5;


            return result;
        }
    };

};

class ZExpressionNode :
    public ZScriptNode
{
public:

    //void AddElement(ExpressionElement element);
    void SetExpression(Expression expr);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:

    Expression mExpression;

};

