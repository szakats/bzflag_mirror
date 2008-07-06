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
  double fl;
  ProductVector* pv;
  Product* p;
  OperationVector* ov;
  StringVector* ids;
  ExpressionVector* ev;
  Operation* o;
  Expression* e;
}
%start ruleset
%token TRANSLATER TRANSLATE SCALE TEST ROUND NEG ASSERTION FACE TAPER SPAWN CHAMFER 
%token TEXTURE TEXTUREFULL TEXTUREQUAD TEXTURECLEAR MATERIAL LOADMATERIAL LOADMATERIALNR
%token SPAWNNGON UNCHAMFER ASSIGN DEFSIGN EXTRUDE EXTRUDET EXPAND RANDOM 
%token REPEATH REPEATV SPLITV SPLITH
%token MULTIFACE FREE NGON REMOVE ADDFACE DETACHFACE 
%token DRIVETHROUGH
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
%type <ids> faces faceparam 
%type <ev> splitparams
%%
ruleset : /* empty */
  | ruleset NONTERM products ';' { 
    String name = String($2);
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
  | DEFSIGN cond ops { $$ = new Product($3,1.0,$2); }
;
faces : /* empty */ { $$ = new StringVector(); }
  | faces NONTERM  { String name = String($2); $$->push_back(name); }
  | faces '*' { String name = String(""); $$->push_back(name); }
;
faceparam : /* empty */ { $$ = NULL; }
  | '[' '@' NONTERM ']' { String name = '@'+String($3); $$ = new StringVector(); $$->push_back(name); }
  | '[' faces ']' { $$ = $2; }
;
ops : /* empty */ { $$ = new OperationVector(); }
  | ops op { $$ = $1; $$->push_back($2); }
;
splitparams : /* empty */  { $$ = new ExpressionVector(); }
  | splitparams expr { $$->push_back($2); }
;
op : EXTRUDE '(' expr ')' faceparam { $$ = new OperationExtrude(ruleset,$3,$5); }
  | EXTRUDET '(' expr ')' faceparam { $$ = new OperationExtrudeT(ruleset,$3,$5); }
  | EXPAND '(' expr ')' { $$ = new OperationExpand(ruleset,$3); }
  | TAPER '(' expr ')' { $$ = new OperationTaper(ruleset,$3); }
  | CHAMFER '(' expr ')' { $$ = new OperationChamfer(ruleset,$3); }
  | ASSERTION '(' expr ')' { $$ = new OperationAssert(ruleset,$3); }
  | UNCHAMFER '(' ')' { $$ = new OperationUnchamfer(ruleset); }
  | DRIVETHROUGH '(' ')' { $$ = new OperationDriveThrough(ruleset); }
  | TEXTURE '(' ')' { $$ = new OperationTexture(ruleset); }
  | TEXTUREFULL '(' ')' { $$ = new OperationTextureFull(ruleset); }
  | TEXTURECLEAR '(' ')' { $$ = new OperationTextureClear(ruleset); }
  | TEXTUREQUAD '(' expr ',' expr ',' expr ',' expr ')' { $$ = new OperationTextureQuad(ruleset,$3,$5,$7,$9); }
  | SPLITH '(' splitparams ')' faceparam { $$ = new OperationSplitFace(ruleset,true,$5,$3); }
  | SPLITV '(' splitparams ')' faceparam { $$ = new OperationSplitFace(ruleset,false,$5,$3); }
  | SPLITH '(' splitparams ',' expr ')' faceparam { $$ = new OperationSplitFace(ruleset,true,$7,$3,$5); }
  | SPLITV '(' splitparams ',' expr ')' faceparam { $$ = new OperationSplitFace(ruleset,false,$7,$3,$5); }
  | REPEATH '(' expr ')' faceparam { $$ = new OperationRepeat(ruleset,$3,true,$5); }
  | REPEATV '(' expr ')' faceparam { $$ = new OperationRepeat(ruleset,$3,false,$5); }
  | SCALE '(' expr ',' expr ')' { $$ = new OperationScale(ruleset,$3,$5); }
  | TRANSLATE '(' expr ',' expr ',' expr ')' { $$ = new OperationTranslate(ruleset,$3,$5,$7); }
  | TRANSLATER '(' expr ',' expr ',' expr ')' { $$ = new OperationTranslateR(ruleset,$3,$5,$7); }
  | MATERIAL '(' expr ')' { $$ = new OperationMaterial(ruleset,$3); }
  | LOADMATERIALNR '(' NONTERM ',' NONTERM ')' { $$ = new OperationLoadMaterial(ruleset,$3,$5,true); }
  | LOADMATERIAL '(' NONTERM ',' NONTERM ')' { $$ = new OperationLoadMaterial(ruleset,$3,$5,false); }
  | MULTIFACE '(' ')' { $$ = new OperationMultiFace(ruleset); }
  | FREE '(' ')' { $$ = new OperationFree(ruleset); }
  | REMOVE '(' ')' { $$ = new OperationRemove(ruleset); }
  | NGON '(' expr ',' expr ')' { $$ = new OperationNGon(ruleset,$3,$5); }
  | NGON '(' expr ')' { $$ = new OperationNGon(ruleset,$3); }
  | ASSIGN '(' NONTERM '=' expr ')' { $$ = new OperationAssign(ruleset,$5,$3); }
  | SPAWN '(' NONTERM ')' { $$ = new OperationSpawn(ruleset,$3); }
  | ADDFACE '(' NONTERM ')' { $$ = new OperationAddFace(ruleset,$3); }
  | DETACHFACE '(' expr ')' faceparam { $$ = new OperationDetachFace(ruleset,$3,$5); }
  | NONTERM { $$ = new OperationNonterminal(ruleset,$1); }
;
expr : RANDOM '(' expr ',' expr ',' expr ')' { $$ = new ExpressionRandom($3,$5,$7); }
  | '(' expr ')'   { $$ = $2; }
  | NEG '(' expr ')' { $$ = new ExpressionNeg($3); }
  | ROUND '(' expr ')' { $$ = new ExpressionRound($3); }
  | expr '-' expr { $$ = new ExpressionSub($1,$3); }
  | expr '+' expr { $$ = new ExpressionAdd($1,$3); }
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
