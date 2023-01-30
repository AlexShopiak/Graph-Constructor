//#include "stdafx.h"
#include < iostream >
#include < cstdlib >
#include < cctype >
#include < cstring >
#include < math.h > 
 

using namespace std;

enum types { DELIMITER = 1, VARIABLE, NUMBER, FUNCTION };
const int NUMVARS = 26;

class Parser {
    static Parser* p_instance;
    Parser();
    Parser(const Parser&);
    Parser& operator = (Parser&);

    char* exp_ptr; // pointer to expression
    char token[256]; // current token
    char tok_type; // type of token
    double vars[NUMVARS]; // variables
    void eval_exp1(double& result); //start proccesing our expression
    void eval_exp2(double& result); //for addition and subtraction
    void eval_exp3(double& result); //for multiplication and division
    void eval_exp4(double& result); //to the power
    void eval_exp5(double& result); //unar plus and minus
    void eval_exp6(double& result); //for function, expr in brackets or Õ variable
    void get_token(); //get next token
public:
    ~Parser();
    static Parser* getInstance();

    char errormsg[64]; //for errors
    double eval_exp(char* exp);//entry point to the parser
};
