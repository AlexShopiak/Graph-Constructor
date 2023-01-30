#include "framework.h"
#include "parser.h"

Parser* Parser::p_instance = 0;

Parser* Parser::getInstance() {
    if (!p_instance) {
        p_instance = new Parser();
    }
    return p_instance;
}

Parser::Parser() {
    exp_ptr = NULL;
    for (int i = 0; i < NUMVARS; i++) {
        vars[i] = 0.0;
        errormsg[0] = '\0';
    }
}

double Parser::eval_exp(char* exp) {
    errormsg[0] = '\0';
    double result;
    exp_ptr = exp;
    get_token();
    if (!*token) {
        strcpy_s(errormsg, "No Expression Present");
        return (double)0;
    }
    eval_exp1(result);
    if (*token) {  // last token must be null                     
        strcpy_s(errormsg, "Syntax Error");
        return result;
    }
}

void Parser::eval_exp1(double& result) {
    int slot;
    char temp_token[80];
    if (tok_type == VARIABLE) {// save old token

        char* t_ptr = exp_ptr;
        strcpy_s(temp_token, token); // calculate index of X in expr
        slot = *token - 'A';
        get_token();

        if (*token != '=') {
            exp_ptr = t_ptr; // current token
            strcpy_s(token, temp_token); // restore old token
            tok_type = VARIABLE;
        }
        else {
            get_token(); // get next part of expr
            eval_exp2(result);
            vars[slot] = result;
            return;
        }
    }
    eval_exp2(result);
}

void Parser::eval_exp2(double& result) {
    register char op;
    double temp;
    eval_exp3(result);
    while ((op = *token) == '+' || op == '-') {
        get_token();
        eval_exp3(temp);
        switch (op)
        {
        case '-':
            result = result - temp;
            break;
        case '+':
            result = result + temp;
            break;
        }
    }
}

void Parser::eval_exp3(double& result) {
    register char op;
    double temp;
    eval_exp4(result);
    while ((op = *token) == '*' || op == '/') {
        get_token();
        eval_exp4(temp);
        switch (op)
        {
        case '*':
            result = result * temp;
            break;
        case '/':
            result = result / temp;
            break;
        }
    }
}

void Parser::eval_exp4(double& result) {
    double temp;
    eval_exp5(result);
    while (*token == '^') {
        get_token();
        eval_exp5(temp);
        result = pow(result, temp);
    }
}

void Parser::eval_exp5(double& result) {
    register char op;
    op = 0;
    if ((tok_type == DELIMITER) && *token == '+' || *token == '-') {
        op = *token;
        get_token();
    }
    eval_exp6(result);
    if (op == '-') {
        result = -result;
    }
}

void Parser::eval_exp6(double& result) {
    bool isfunc = (tok_type == FUNCTION);
    char temp_token[80];
    if (isfunc) {
        strcpy_s(temp_token, token);
        get_token();
    }
    if ((*token == '(')) {
        get_token();
        eval_exp2(result);
        if (*token != ')') {
            strcpy_s(errormsg, "Unbalanced Parentheses");
        }
        if (isfunc) {
            if (!strcmp(temp_token, "SIN"))
                result = sin(result);
            else if (!strcmp(temp_token, "COS"))
                result = cos(result);
            else if (!strcmp(temp_token, "TAN"))
                result = tan(result);
            else if (!strcmp(temp_token, "LN"))
                result = log(result);
            else if (!strcmp(temp_token, "LOG"))
                result = log10(result);
            else if (!strcmp(temp_token, "EXP"))
                result = exp(result);
            else if (!strcmp(temp_token, "SQRT"))
                result = sqrt(result);
            else if (!strcmp(temp_token, "SQR"))
                result = result * result;
            else
                strcpy_s(errormsg, "Unknown Function");
        }
        get_token();
    }
    else
        switch (tok_type)
        {
        case VARIABLE:
            result = vars[*token - 'A'];
            get_token();
            return;
        case NUMBER:
            result = atof(token);
            get_token();
            return;
        default:
            strcpy_s(errormsg, "Syntax Error");
        }
}

void Parser::get_token() {
    register char* temp;
    tok_type = 0;
    temp = token;
    *temp = '\0';
    if (!*exp_ptr) { // end of exp
        return;
    }
    while (isspace(*exp_ptr)) {  // skip spaces
        ++exp_ptr;
    }
    if (strchr("+-*/%^=()", *exp_ptr)) {
        tok_type = DELIMITER;
        *temp++ = *exp_ptr++;  // to the next char
    }
    else if (isalpha(*exp_ptr)) {
        while (!strchr(" +-/*%^=()\t\r", *exp_ptr) && (*exp_ptr)) {
            *temp++ = toupper(*exp_ptr++);
        }
        while (isspace(*exp_ptr)) // skip spaces
            ++exp_ptr;
        tok_type = (*exp_ptr == '(') ? FUNCTION : VARIABLE;
    }
    else if (isdigit(*exp_ptr) || *exp_ptr == '.') {
        while (!strchr(" +-/*%^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        tok_type = NUMBER;
    }
    *temp = '\0';
    if ((tok_type == VARIABLE) && (token[1])) {
        strcpy_s(errormsg, "Only first letter of variables is considered");
    }
}