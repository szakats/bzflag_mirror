%{
#include "parser.cxx.h"
#include <memory>
#define YY_NEVER_INTERACTIVE 1
%}
DIGIT [0-9]
ID [a-z][a-z0-9]*
%%
"->"                { return(DEFSIGN);  }
{DIGIT}+            { return(NUMBER);   }
{DIGIT}+"."{DIGIT}* { return(NUMBER);   }
extrude             { return(EXTRUDE);  }
expand              { return(EXPAND);   }
taper               { return(TAPER);    }
random              { return(RANDOM);   }
subdivide           { return(SUBDIVIDE);}
{ID}                { return(NONTERM);  }
#[^\n]*
[ \t\n]+
.                   { return(yytext[0]); }
%%

int yywrap () {
  return 1;
}

void yyerror (char *s) 
{
  printf ("%s\n", s);
}
