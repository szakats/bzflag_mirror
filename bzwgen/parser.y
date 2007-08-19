%{
#include <memory>
#include <string>
#include "globals.h"
#include "Rule.h"
#include "RuleSet.h"
#include "Product.h"
#include "Operation.h"
#include "Expression.h"

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
  StringVector* ids;
  Operation* o;
  Expression* e;
}
%start ruleset
%token TAPER SPAWN CHAMFER UNCHAMFER ASSIGN DEFSIGN EXTRUDE EXPAND RANDOM SUBDIVIDEH SUBDIVIDEV PARTITIONH PARTITIONV MATERIAL
%token <fl> NUMBER
%token <id> NONTERM ATTRIBUTE
%type <pv> products
%type <p> product
%type <ov> ops
%type <o> op
%type <e> expr
%type <ids> faces faceparam singleface
%%
ruleset : /* empty */
  | ruleset NONTERM products ';' { 
    std::string name = std::string($2);
    ruleset->addRule(name,new Rule(name,$3));
  }
;
products : /* empty */ { $$ = new ProductVector(); }
  | products product { $$ = $1; $$->push_back($2); }
;
product : DEFSIGN NUMBER ':' ops { $$ = new Product($4,$2); }
  | DEFSIGN ops { $$ = new Product($2); }
;
faces : /* empty */ { $$ = new StringVector(); }
  | faces NONTERM  { std::string name = std::string($2); $$->push_back(name); }
  | faces '*' { std::string name = std::string(""); $$->push_back(name); }
;
faceparam : /* empty */ { $$ = NULL; }
  | '[' '@' NONTERM ']' { std::string name = '@'+std::string($3); $$ = new StringVector(); $$->push_back(name); }
  | '[' faces ']' { $$ = $2; }
;
singleface : /* empty */ { $$ = NULL; }
  | '[' '@' NONTERM ']' { std::string name = std::string($3); $$ = new StringVector(); $$->push_back(name); }
  | '[' NONTERM ']' { std::string name = std::string($2); $$ = new StringVector(); $$->push_back(name); }
  ;
ops : /* empty */ { $$ = new OperationVector(); }
  | ops op { $$ = $1; $$->push_back($2); }
;
op : EXTRUDE '(' expr ')' faceparam { $$ = new OperationExtrude(ruleset,$3,$5); }
  | EXPAND '(' expr ')' { $$ = new OperationExpand(ruleset,$3); }
  | TAPER '(' expr ')' { $$ = new OperationTaper(ruleset,$3); }
  | CHAMFER '(' expr ')' { $$ = new OperationChamfer(ruleset,$3); }
  | UNCHAMFER '(' ')' { $$ = new OperationUnchamfer(ruleset); }
  | SUBDIVIDEH '(' expr ')' faceparam { $$ = new OperationSubdivide(ruleset,$3,true,$5); }
  | SUBDIVIDEV '(' expr ')' faceparam { $$ = new OperationSubdivide(ruleset,$3,false,$5); }
  | PARTITIONH '(' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,true,$5); }
  | PARTITIONV '(' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,false,$5); }
  | MATERIAL '(' expr ')' { $$ = new OperationMaterial(ruleset,$3); }
  | ASSIGN '(' NONTERM '=' expr ')' { std::string name = std::string($3); $$ = new OperationAssign(ruleset,$5,name); }
  | SPAWN '(' NONTERM ')' { std::string name = std::string($3); $$ = new OperationSpawn(ruleset,name); }
  | NONTERM { std::string name = std::string($1); $$ = new OperationNonterminal(ruleset,name); }
;
expr : RANDOM '(' expr ',' expr ',' expr ')' { $$ = new ExpressionRandom($3,$5,$7); }
  | NUMBER { $$ = new ExpressionConst($1); }
  | ATTRIBUTE { std::string name = std::string($1); $$ = new ExpressionAttribute(ruleset,name); }
  ;
%%