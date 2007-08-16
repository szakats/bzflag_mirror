%{
#include <memory>
#include <string>
#include "Rule.h"
#include "RuleSet.h"
#include "Product.h"

void yyerror(RuleSet *ruleset, char* s);
int yylex();
%}
%parse-param {RuleSet *ruleset}%union _YYSTYPE {
  char *id;
  float fl;
  ProductVector* pv;
  Product* p;
}
%start ruleset
%token DEFSIGN EXTRUDE EXPAND TAPER RANDOM SUBDIVIDE
%token NUMBER
%token <id> NONTERM
%type <pv> products
%type <p> product
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
ops : /* empty */
  | ops op
;
op : EXTRUDE '(' expr ')'
  | EXPAND '(' expr ')'
  | TAPER '(' expr ')'
  | SUBDIVIDE '(' expr ')'
  | NONTERM {  }
;
expr : RANDOM '(' expr ',' expr ',' expr ')'
  | NUMBER
  ;
%%