%start ruleset
%token DEFSIGN EXTRUDE EXPAND TAPER RANDOM SUBDIVIDE
%token NUMBER
%token NONTERM
%%
ruleset : /* empty */
  | ruleset NONTERM products ';'
;
products : /* empty */
  | products product
;
product : DEFSIGN NUMBER ':' ops
  | DEFSIGN ops
;
ops : /* empty */
  | ops op
;
op : EXTRUDE '(' expr ')'
  | EXPAND '(' expr ')'
  | TAPER '(' expr ')'
  | SUBDIVIDE '(' expr ')'
  | NONTERM
;
expr : RANDOM '(' expr ',' expr ',' expr ')'
  | NUMBER
  ;
%%