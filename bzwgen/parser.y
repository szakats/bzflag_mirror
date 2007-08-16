%{
#include <memory>
#include <string>
#include "Rule.h"
#include "RuleSet.h"
#include "Product.h"
#include "Operation.h"

void yyerror(RuleSet *ruleset, char* s);
int yylex();
void yyunput(int, char*);
%}
%parse-param {RuleSet *ruleset}%union _YYSTYPE {
  char *id;
  float fl;
  ProductVector* pv;
  Product* p;
  OperationVector* ov;
  Operation* o;
}
%start ruleset
%token DEFSIGN EXTRUDE EXPAND TAPER RANDOM SUBDIVIDE
%token NUMBER
%token <id> NONTERM
%type <pv> products
%type <p> product
%type <ov> ops
%type <o> op
%%
ruleset : /* empty */
  | ruleset NONTERM products ';' { 
    std::string name = std::string($2);
    ruleset->addRule(name,new Rule(name));
  }
;
products : /* empty */ { $$ = new ProductVector(); }
  | products product { $$ = $1; $$->push_back($2); }
;
product : DEFSIGN NUMBER ':' ops { $$ = new Product(); }
  | DEFSIGN ops { $$ = new Product(); }
;
ops : /* empty */ { $$ = new OperationVector(); }
  | ops op { $$ = $1; $$->push_back($2); }
;
op : EXTRUDE '(' expr ')' { $$ = new Operation(); }
  | EXPAND '(' expr ')' { $$ = new Operation(); }
  | TAPER '(' expr ')' { $$ = new Operation(); }
  | SUBDIVIDE '(' expr ')' { $$ = new Operation(); }
  | NONTERM { $$ = new Operation(); }
;
expr : RANDOM '(' expr ',' expr ',' expr ')'
  | NUMBER
  ;
%%