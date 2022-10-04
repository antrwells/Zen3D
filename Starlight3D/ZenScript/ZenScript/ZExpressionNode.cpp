#include "ZExpressionNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"
#include "ZClassNode.h"
#include "ZStatementNode.h"
#include "ZClassStatementNode.h"
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

    if (op == OpPlus || op == OpMinus)
        return 3;
    if (op == OpMultiply || op == OpDivide)
        return 5;
    if (op == OpGreater || op == OpLess)
        return 8;
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
    }
}
float applyOpFloat(float a, float b, ExprOperatorType op) {
    switch (op) {
    case OpPlus: return a + b;
    case OpMinus: return a - b;
    case OpMultiply: return a * b;
    case OpDivide: return a / b;
    case OpGreater: return a > b ? 1 : 0;
    case OpLess: return a < b ?  1 : 0;
    }
}


int evaluateInt(std::vector<ExpressionElement> mElements) {

    std::stack<int> values;
    std::stack<ExprOperatorType> ops;

    for (int i = 0; i < mElements.size(); i++) {

        auto tok = mElements[i];

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
                    values.push(evar->GetIntVal());
                }
            }
        }
        else if (tok.mOp == OpRightBrace)
        {
            while (!ops.empty() && ops.top()!=OpLeftBrace)
            {
      
                int val2 = values.top();
                values.pop();

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

                int val1 = values.top();
                values.pop();

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

        if (tok.mOp == ExprOperatorType::OpLeftBrace)
        {
            ops.push(tok.mOp);

        }
        else if (tok.mType == EInt || tok.mType == EFloat || tok.mType == EVar)
        {
            if (tok.mType == EInt) {
                values.push((float)tok.mValInt);
            }
            else if (tok.mType == EFloat) {
                values.push(tok.mValFloat);
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
                    if (evar->GetType() == VarType::VarFloat) {
                        values.push(evar->GetFloatVal());
                    }
                    else {
                        values.push((float)evar->GetIntVal());
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

                float val1 = values.top();
                values.pop();

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


        //--
        ZContextVar* result = new ZContextVar("expr result", VarType::VarInt);

        result->SetInt(evaluateInt(mElements));
        //        result->SetInt(101);
        int aa = 5;


        return result;

    };

}