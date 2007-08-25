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
%parse-param {RuleSet *ruleset}
%union _YYSTYPE {
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
%token TEST ROUND NEG ASSERTION FACE TAPER SPAWN CHAMFER TEXTURE TEXTUREFULL TEXTUREQUAD TEXTURECLEAR
%token SPAWNNGON UNCHAMFER ASSIGN DEFSIGN EXTRUDE EXTRUDET EXPAND RANDOM MATERIAL
%token SUBDIVIDEH SUBDIVIDEV PARTITIONH PARTITIONV PARTITIONHI PARTITIONVI 
%token MULTIFACE
%token <fl> NUMBER
%token <id> NONTERM ATTRIBUTE
%left '&' '|'
%nonassoc '<' '>' '='
%left '-' '+'
%left '*' '/'
%type <pv> products
%type <p> product
%type <ov> ops
%type <o> op
%type <e> expr cond
%type <ids> faces faceparam singleface
%%
ruleset : /* empty */
  | ruleset NONTERM products ';' { 
    std::string name = std::string($2);
    ruleset->addRule(name,new Rule(name,$3));
  }
;
cond : /* empty */ { $$ = NULL; }
  | '(' expr ')' { $$ = $2; }
  ;
products : /* empty */ { $$ = new ProductVector(); }
  | products product { $$ = $1; $$->push_back($2); }
;
product : DEFSIGN NUMBER ':' cond ops { $$ = new Product($5,$2,$4); }
  | DEFSIGN cond ops { $$ = new Product($3,1.0f,$2); }
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
  | EXTRUDET '(' expr ')' faceparam { $$ = new OperationExtrudeT(ruleset,$3,$5); }
  | EXPAND '(' expr ')' { $$ = new OperationExpand(ruleset,$3); }
  | TAPER '(' expr ')' { $$ = new OperationTaper(ruleset,$3); }
  | CHAMFER '(' expr ')' { $$ = new OperationChamfer(ruleset,$3); }
  | ASSERTION '(' expr ')' { $$ = new OperationAssert(ruleset,$3); }
  | UNCHAMFER '(' ')' { $$ = new OperationUnchamfer(ruleset); }
  | TEXTURE '(' ')' { $$ = new OperationTexture(ruleset); }
  | TEXTUREFULL '(' ')' { $$ = new OperationTextureFull(ruleset); }
  | TEXTURECLEAR '(' ')' { $$ = new OperationTextureClear(ruleset); }
  | TEXTUREQUAD '(' expr ',' expr ',' expr ',' expr ')' { $$ = new OperationTextureQuad(ruleset,$3,$5,$7,$9); }
  | SUBDIVIDEH '(' expr ')' faceparam { $$ = new OperationSubdivide(ruleset,$3,true,$5); }
  | SUBDIVIDEV '(' expr ')' faceparam { $$ = new OperationSubdivide(ruleset,$3,false,$5); }
  | SUBDIVIDEH '(' expr ',' expr ')' faceparam { $$ = new OperationSubdivide(ruleset,$3,true,$7,$5); }
  | SUBDIVIDEV '(' expr ',' expr ')' faceparam { $$ = new OperationSubdivide(ruleset,$3,false,$7,$5); }
  | PARTITIONH '(' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,true,$5); }
  | PARTITIONV '(' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,false,$5); }
  | PARTITIONHI '(' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,true,$5,true); }
  | PARTITIONVI '(' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,false,$5,true); }
  | PARTITIONH '(' expr ',' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,true,$7,false,$5); }
  | PARTITIONV '(' expr ',' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,false,$7,false,$5); }
  | PARTITIONHI '(' expr ',' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,true,$7,true,$5); }
  | PARTITIONVI '(' expr ',' expr ')' singleface { $$ = new OperationPartition(ruleset,$3,false,$7,true,$5); }
  | MATERIAL '(' expr ')' { $$ = new OperationMaterial(ruleset,$3); }
  | MULTIFACE '(' ')' { $$ = new OperationMultiFace(ruleset); }
  | ASSIGN '(' NONTERM '=' expr ')' { $$ = new OperationAssign(ruleset,$5,$3); }
  | TEST '(' ')' { $$ = new OperationTest(ruleset); }
  | SPAWN '(' NONTERM ')' { $$ = new OperationSpawn(ruleset,$3); }
  | SPAWNNGON '(' NONTERM ',' expr ')' { $$ = new OperationSpawnNGon(ruleset,$3,$5); }
  | NONTERM { $$ = new OperationNonterminal(ruleset,$1); }
;
expr : RANDOM '(' expr ',' expr ',' expr ')' { $$ = new ExpressionRandom($3,$5,$7); }
  | '(' expr ')'   { $$ = $2; }
  | NEG '(' expr ')' { $$ = new ExpressionNeg($3); }
  | ROUND '(' expr ')' { $$ = new ExpressionRound($3); }
  | expr '-' expr { $$ = new ExpressionSub($1,$3); }
  | expr '*' expr { $$ = new ExpressionMult($1,$3); }
  | expr '/' expr { $$ = new ExpressionDiv($1,$3); }
  | expr '>' expr { $$ = new ExpressionGreater($1,$3); }
  | expr '<' expr { $$ = new ExpressionGreater($3,$1); }
  | expr '&' expr { $$ = new ExpressionAnd($1,$3); }
  | expr '=' expr { $$ = new ExpressionEqual($1,$3); }
  | expr '|' expr { $$ = new ExpressionOr($1,$3); }
  | FACE '(' NONTERM ')' { $$ = new ExpressionFaceAttribute($3); }
  | NUMBER { $$ = new ExpressionConst($1); }
  | ATTRIBUTE { $$ = new ExpressionAttribute(ruleset,$1); }
  ;
%%
