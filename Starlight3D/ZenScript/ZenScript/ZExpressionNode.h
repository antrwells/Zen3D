#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>
#include <stack>
#include "ZContextVar.h"
#include "VarTypes.h"

enum ExprOperatorType {

    OpPlus, OpMinus, OpMultiply, OpDivide, OpLeftBrace, OpRightBrace,OpGreater,OpLess,OpEquals,OpNot,OpAnd,OpOr,OpUMinus

};

enum ExprElementType {

    EInt,EFloat,EString,EOp,SubExpression,EVar,EStatement,EClassStatement,ENew

};

class ZNewNode;
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
    ZNewNode* mNew;

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

bool IsString(ExpressionElement e);

class Expression
{
public:
    
    std::vector<ExpressionElement> mElements;
    ZContextVar* Evaluate(VarType recv);
    bool HasCObj() {
        for (int i = 0; i < mElements.size(); i++) {
            if (mElements[i].mType == EOp)
            {
                return true;
            }
        }
        return false;


    }
    bool HasOperators() {
        for (int i = 0; i < mElements.size(); i++) {
            if (mElements[i].mType == EOp)
            {
                return true;
            }
        }
        return false;
    }
    bool IsStrings() {
        for (int i = 0; i < mElements.size(); i++) {
            if (mElements[i].mType == EString) {
                return true;
            }
            if (IsString(mElements[i]))
            {
                return true;
            }
        }
        return false;
    }
    bool IsCompare();
    bool DoesCompare();
    bool IsInt() {
        bool rv = true;
        bool o_int = false;

        for (int i = 0; i < mElements.size(); i++)
        {
            if (mElements[i].mType == EString) {
                return false;
            }
            if (mElements[i].mType == EInt)
            {
                o_int = true;
                return true;
            }
            if (mElements[i].mType == EVar)
            {
               
                return false;

            }
            else if (mElements[i].mType == EFloat)
            {
                return false;
            }


        }
        return o_int;
    }

    bool IsFloat() {
        bool rv = true;
        bool o_flt = false;

        for (int i = 0; i < mElements.size(); i++)
        {
            if (mElements[i].mType == EString) {
                return false;
            }
            if (mElements[i].mType == EFloat)
            {
                o_flt = true;
            }
            else if (mElements[i].mType == EInt)
            {
                return false;
            }
            

        }
        return o_flt;
    }

    bool IsVar() {
      
    }

    bool IsCObj() {

    }

    bool IsStatic() {

    }
    

};

class ZExpressionNode :
    public ZScriptNode
{
public:
    static VarType RecvType;
    //void AddElement(ExpressionElement element);
    void SetExpression(Expression expr);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    Expression GetExpression();

private:

    Expression mExpression;

};

