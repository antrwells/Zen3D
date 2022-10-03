#include "ZExpressionNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"
#include "ZClassNode.h"
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
        return 1;
    if (op == OpMultiply || op == OpDivide)
        return 2;
    return 0;
}

int applyOpInt(int a, int b, ExprOperatorType op) {
    switch (op) {
    case OpPlus: return a + b;
    case OpMinus: return a - b;
    case OpMultiply: return a * b;
    case OpDivide: return a / b;
    }
}
float applyOpFloat(float a, float b, ExprOperatorType op) {
    switch (op) {
    case OpPlus: return a + b;
    case OpMinus: return a - b;
    case OpMultiply: return a * b;
    case OpDivide: return a / b;
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
        else if (tok.mType == EInt || tok.mType == EFloat || tok.mType == EVar)
        {
            //int val = 0;
            if (tok.mType == EInt) {
                values.push(tok.mValInt);
            }
            else if(tok.mType==EFloat) {
                values.push((int)tok.mValFloat);
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
        else if (tok.mType == EInt || tok.mType == EFloat)
        {
            //int val = 0;
            if (tok.mType == EFloat) {
                values.push(tok.mValFloat);
            }
            else {
                values.push((float)tok.mValInt);
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