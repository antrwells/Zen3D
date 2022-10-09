#include "ZExpressionNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"
#include "ZClassNode.h"
#include "ZStatementNode.h"
#include "ZClassStatementNode.h"
#include "ZContextVar.h"
void ZExpressionNode::SetExpression(Expression expr) {

	//mElements.push_back(element);


    

	mExpression = expr;

}

ZContextVar* ZExpressionNode::Exec(const std::vector<ZContextVar*>& params)
{
	return mExpression.Evaluate();

	return nullptr;
}



int precedence(ExprOperatorType op) {

    if (op == OpUMinus)
        return 3;
    
    if (op == OpPlus || op == OpMinus)
        return 6;
    if (op == OpMultiply || op == OpDivide)
        return 5;
    if (op == OpGreater || op == OpLess)
        return 8;
    if (op == OpEquals || op == OpNot)
        return 9;
    if (op == OpAnd)
        return 13;
    if (op == OpOr)
        return 14;
    

    return 0;
}

int applyOpInt(int a, int b, ExprOperatorType op) {
    switch (op) {
       case OpPlus: return a + b;
    case OpMinus: return a - b;
    case OpMultiply: return a * b;
    case OpDivide: return a / b;
    case OpGreater: return a > b ? 1 : 0;
    case OpLess: return a < b ? 1 : 0;
    case OpEquals: return a == b ? 1 : 0;
    case OpNot: return a != b ? 1 : 0;
    case OpAnd:
    {
        bool res = ((a >= 1) && (b >= 1));
        if (res) return 1;
        return 0;
        break;
    }
    case OpOr:
    {
        bool res = ((a >= 1)) || (b >= 1);
        if (res) return 1;
        return 0;
        break;
    }

    }
    int bbb = 5;
}

float applyOpFloat(float a, float b, ExprOperatorType op) {
    switch (op) {
    case OpPlus: return a + b;
    case OpMinus: return a - b;
    case OpMultiply: return a * b;
    case OpDivide: return a / b;
    case OpGreater: return a > b ? 1 : 0;
    case OpLess: return a < b ? 1 : 0;
    case OpEquals: return a == b ? 1 : 0;
    case OpNot: return a != b ? 1 : 0;
    case OpAnd:
    {
        bool res = ((a >= 1) && (b >= 1));
        if (res) return 1;
        return 0;
        break;
    }
    case OpOr:
    {
        bool res = ((a >= 1)) || (b >= 1);
        if (res) return 1;
        return 0;
        break;
    }

    }
    int bbb = 5;
}


int evaluateInt(std::vector<ExpressionElement> mElements) {

    std::stack<int> values;
    std::stack<ExprOperatorType> ops;

    for (int i = 0; i < mElements.size(); i++) {

        auto tok = mElements[i];

        if (tok.mOp == OpUMinus) {
            values.push(-mElements[i + 1].mValInt);
            i++;
            continue;
        };

     

        if (tok.mOp == ExprOperatorType::OpLeftBrace)
        {
            ops.push(tok.mOp);

        }
        else if (tok.mType == EInt || tok.mType == EFloat || tok.mType == EVar || tok.mType == EStatement || tok.mType == EClassStatement)
        {
            //int val = 0;
            if (tok.mType == EInt) {
                values.push(tok.mValInt);
            }
            else if(tok.mType==EFloat) {
                values.push((int)tok.mValFloat);
            }
            else if (tok.mType == EClassStatement)
            {
                auto res = tok.mClassStatement->Exec({});

                switch (res->GetType()) {
                case VarType::VarInt:
                    values.push(res->GetIntVal());
                    break;
                case VarType::VarFloat:
                    values.push((int)res->GetFloatVal());
                    break;
                }
                int aa = 5;
            }
            else if (tok.mType == EStatement)
            {
                auto res = tok.mStatement->Exec(std::vector<ZContextVar*>());
                
                switch (res->GetType()) {
                case VarType::VarInt:
                    values.push(res->GetIntVal());
                    break;
                case VarType::VarFloat:
                    values.push((int)res->GetFloatVal());
                    break;
                }

                //values.push(res->GetIntVal());


            }
            else if (tok.mType == EVar)
            {
                int depth = 0;
                for (int i = 0; i < 16; i++)
                {
                    if (tok.mValName[i] != "")
                    {
                        depth++;
                    }
                }

                if (depth > 1)
                {
                    auto cls = ZScriptContext::CurrentContext->GetScope()->FindVar(tok.mValName[0]);
                    auto av = cls->GetClassVal()->FindVar(tok.mValName[1]);
                    if (av->GetType() == VarType::VarInt)
                    {

                        values.push(av->GetIntVal());
                    }
                    else {
                        values.push(av->GetFloatVal());
                    }                    //values.push(333);
                }
                else {
                    auto evar = ZScriptContext::CurrentContext->GetScope()->FindVar(tok.mValName[0]);
                    switch (evar->GetType()) {
                    case VarType::VarFloat:
                        values.push((int)evar->GetFloatVal());
                        break;
                    case VarType::VarInt:
                        values.push(evar->GetIntVal());
                        break;
                    }
                }
            }
        }
        else if (tok.mOp == OpRightBrace)
        {
            while (!ops.empty() && ops.top()!=OpLeftBrace)
            {
      
                int val2 = values.top();
                values.pop();

                if (values.size() == 0) {
                    //if()
                }

                int val1 = values.top();
                values.pop();

                ExprOperatorType op = ops.top();
                ops.pop();

                values.push(applyOpInt(val1, val2, op));

            }

            if (!ops.empty()) {
                ops.pop();
            }

        }
        else {

            while (!ops.empty() && precedence(ops.top())
                >= precedence(tok.mOp)) {
                int val2 = values.top();
                values.pop();

                int val1 = 0;
                if (values.size() == 0) {

                }
                else {
                    val1 = values.top();
                    values.pop();
                }
                ExprOperatorType op = ops.top();
                ops.pop();

                values.push(applyOpInt(val1, val2, op));
            }

            ops.push(tok.mOp);

        }

        //if(tok.mType == ExprElementType::


    }

    while (!ops.empty()) {
        int val2 = values.top();
        values.pop();

        if (values.size() == 0)
        {
            if (ops.top() == OpMinus) {
                values.push(-val2);
                ops.pop();
                continue;
            }
            int aa = 5;
        }

        int val1 = values.top();
        values.pop();

        ExprOperatorType op = ops.top();
        ops.pop();

        values.push(applyOpInt(val1, val2, op));
    }

    return values.top();

}


float evaluateFloat(std::vector<ExpressionElement> mElements) {

    std::stack<float> values;
    std::stack<ExprOperatorType> ops;

    for (int i = 0; i < mElements.size(); i++) {

        auto tok = mElements[i];


        if (tok.mOp == OpUMinus) {
            values.push(-mElements[i + 1].mValFloat);
            i++;
            continue;
        };


        if (tok.mOp == ExprOperatorType::OpLeftBrace)
        {
            ops.push(tok.mOp);

        }
        else if (tok.mType == EInt || tok.mType == EFloat || tok.mType == EVar || tok.mType == EStatement || tok.mType == EClassStatement)
        {
            //int val = 0;
            if (tok.mType == EInt) {
                values.push((float)tok.mValInt);
            }
            else if (tok.mType == EFloat) {
                values.push(tok.mValFloat);
            }
            else if (tok.mType == EClassStatement)
            {
                auto res = tok.mClassStatement->Exec({});

                switch (res->GetType()) {
                case VarType::VarInt:
                    values.push((float)res->GetIntVal());
                    break;
                case VarType::VarFloat:
                    values.push(res->GetFloatVal());
                    break;
                }
                int aa = 5;
            }
            else if (tok.mType == EStatement)
            {
                auto res = tok.mStatement->Exec(std::vector<ZContextVar*>());

                switch (res->GetType()) {
                case VarType::VarInt:
                    values.push((float)res->GetIntVal());
                    break;
                case VarType::VarFloat:
                    values.push(res->GetFloatVal());
                    break;
                }

                //values.push(res->GetIntVal());


            }
            else if (tok.mType == EVar)
            {
                int depth = 0;
                for (int i = 0; i < 16; i++)
                {
                    if (tok.mValName[i] != "")
                    {
                        depth++;
                    }
                }

                if (depth > 1)
                {
                    auto cls = ZScriptContext::CurrentContext->GetScope()->FindVar(tok.mValName[0]);
                    auto av = cls->GetClassVal()->FindVar(tok.mValName[1]);
                    if (av->GetType() == VarType::VarInt)
                    {

                        values.push((float)av->GetIntVal());
                    }
                    else {
                        values.push(av->GetFloatVal());
                    }                    //values.push(333);
                }
                else {
                    auto evar = ZScriptContext::CurrentContext->GetScope()->FindVar(tok.mValName[0]);
                    if (evar->GetType() == EInt) {
                        values.push((float)evar->GetIntVal());
                    }
                    else {
                        values.push(evar->GetFloatVal());
                    }
                }
            }
        }
        else if (tok.mOp == OpRightBrace)
        {
            while (!ops.empty() && ops.top() != OpLeftBrace)
            {

                float val2 = values.top();
                values.pop();

                float val1 = values.top();
                values.pop();

                ExprOperatorType op = ops.top();
                ops.pop();

                values.push(applyOpFloat(val1, val2, op));

            }

            if (!ops.empty()) {
                ops.pop();
            }

        }
        else {

            while (!ops.empty() && precedence(ops.top())
                >= precedence(tok.mOp)) {
                float val2 = values.top();
                values.pop();

                float val1 = 0;
                if (values.size() == 0) {

                }
                else {
                    val1 = values.top();
                    values.pop();
                }
                 

                ExprOperatorType op = ops.top();
                ops.pop();

                values.push(applyOpFloat(val1, val2, op));
            }

            ops.push(tok.mOp);

        }

        //if(tok.mType == ExprElementType::


    }

    while (!ops.empty()) {
        float val2 = values.top();
        values.pop();

        float val1 = values.top();
        values.pop();

        ExprOperatorType op = ops.top();
        ops.pop();

        values.push(applyOpFloat(val1, val2, op));
    }

    return values.top();

}

Expression ZExpressionNode::GetExpression() {

    return mExpression;

}

ZContextVar* Expression::Evaluate() {


    if (mElements.size() == 1)
    {
        if (mElements[0].mType == ExprElementType::EStatement) {

            auto vv = mElements[0].mStatement->Exec({});

            return vv;

        }
        if (mElements[0].mType == ExprElementType::EVar)
        {

            auto gvar = ZScriptContext::CurrentContext->GetScope()->FindVar(mElements[0].mValName[0]);

            if (mElements[0].mValName[1] != "")
            {
                auto vv = gvar->GetClassVal();
  //              ZScriptContext::CurrentContext->PushClass(vv);
                auto ret_v = vv->FindVar(mElements[1].mValName[1]);
    //            ZScriptContext::CurrentContext->PopClass();
                return ret_v;
            
            }
            else {
                return gvar;
            }
            int b = 0;
        }

    }

    bool is_int = true;
    bool is_string = false;
    bool is_cs = false;
    for (int i = 0; i < mElements.size(); i++) {
        if (mElements[i].mType == EFloat)
        {
            is_int = false;
        }
        if (mElements[i].mType == EClassStatement)
        {
            is_cs = true;
        }
        if (mElements[i].mType == EString)
        {
            is_string = true;
        }
        if (mElements[i].mType == EInt)
        {

            //VString 
            //str_val + 

        }
        if(mElements[i].mType == EVar)
        {

            auto gvar = ZScriptContext::CurrentContext->GetScope()->FindVar(mElements[i].mValName[0]);
            if (mElements[i].mValName[1] != "")
            {
                auto cv = gvar->GetClassVal();
                auto nv = cv->FindVar(mElements[i].mValName[1]);
                if (nv->GetType() == VarType::VarFloat)
                {
                    is_int = false;
                }
                int aaa = 5;
            }
            else {
                if (gvar->GetType() == VarType::VarFloat) {
                    is_int = false;
                }
            }
            // int aa = 5;
           // is_string = true;

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
            if (mElements[i].mType == EVar)
            {
                auto gvar = ZScriptContext::CurrentContext->GetScope()->FindVar(mElements[i].mValName[0]);
                int aa = 5;
                str_val = str_val + gvar->GetStringVal();
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
    else if (is_cs == false) {

        ZContextVar* result = new ZContextVar("expr result", VarType::VarInt);

        result->SetInt(evaluateInt(mElements));
        return result;

    }
    else
    {

        //determine type

        for (int i = 0; i < mElements.size(); i++)
        {
            auto ele = mElements[i];
            if (ele.mType == EVar)
            {
                int depth = 0;
                for (int i = 0; i < 16; i++)
                {
                    if (ele.mValName[i] != "")
                    {
                        depth++;
                    }
                }

                if (depth > 1)
                {
                    auto cls = ZScriptContext::CurrentContext->GetScope()->FindVar(ele.mValName[0]);
                    auto av = cls->GetClassVal()->FindVar(ele.mValName[1]);
                    if (av->GetType() == VarType::VarFloat)
                    {
                        ZContextVar* result = new ZContextVar("expr result", VarType::VarFloat);

                        result->SetFloat(evaluateFloat(mElements));
                        return result;
                        //  values.push(av->GetIntVal());
                    }
                    else {
                        //    values.push(av->GetFloatVal());
                    }

                    int aa = 5;
                }
            }

        }

        if (is_cs) {
            VarType rt = VarType::VarInt;
            //class statements
            for (int i = 0; i < mElements.size(); i++) {

                if (mElements[i].mType == EClassStatement)
                {
                    auto t = mElements[i].mClassStatement->GetReturnType();

                    rt = t;

                    int bb = 0;

                }

            }

            //--


            if (rt == VarType::VarInt)
            {
                ZContextVar* result = new ZContextVar("expr result", VarType::VarInt);

                result->SetInt(evaluateInt(mElements));
                return result;
            }
            if (rt == VarType::VarFloat) {
                ZContextVar* result = new ZContextVar("expr result", VarType::VarFloat);
                result->SetFloat(evaluateFloat(mElements));
                return result;
            }
        }


        //        result->SetInt(101);
        int aa = 5;

        return nullptr;
        //return result;

    };

}