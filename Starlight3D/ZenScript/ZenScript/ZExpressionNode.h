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

    EInt,EFloat,EString,EOp,SubExpression,EVar

};


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



        bool is_int = true;
        bool is_string = false;
        for (int i = 0; i < mElements.size(); i++) {
            if (mElements[i].mType == EFloat)
            {
                is_int = false;
            }
            if (mElements[i].mType == EString)
            {
                is_string = true;
            }
        }

        if (is_string) {


            std::string str_val = "";
            for (int i = 0; i < mElements.size(); i++)
            {
                if (mElements[i].mType == ExprElementType::EString)
                {
                    str_val = str_val + mElements[i].mValString;
                }
            }


            ZContextVar* result = new ZContextVar("expr result", VarType::VarString);
            result->SetString(str_val);
            return result;

        }

        if (!is_int)
        {
            ZContextVar* result = new ZContextVar("expr result", VarType::VarFloat);

            result->SetFloat(evaluateFloat(mElements));
            //        result->SetInt(101);
            int aa = 5;


            return result;
        }
        else
        {

            //determine type


            //--
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
    Expression GetExpression();

private:

    Expression mExpression;

};

