%{
#include "parser.tab.h"
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
.                   { return(yytext[0]; }
%%
