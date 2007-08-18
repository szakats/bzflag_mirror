%{
#include <memory>
#include <string>
#include "RuleSet.h"
#include "Product.h"
#include "Operation.h"
#include "Expression.h"
#include "parser.hxx"

// Ugly hacks to surpress GCC warnings
#define YY_NEVER_INTERACTIVE 1
#define YY_INPUT(buf,result,max_size) \
	if ( ((result = fread( buf, 1, max_size, yyin )) == 0) \
		  && ferror( yyin ) ) { \
		YY_FATAL_ERROR( "input in flex scanner failed" ); \
		unput(' '); \
	}
// End of hacks.
		
%}
DIGIT [0-9]
ID [a-z][a-z0-9]*
%option yylineno
%%
"->"                  { return(DEFSIGN);  }
-?{DIGIT}+            { yylval.fl = (float)atof(yytext); return(NUMBER);   }
-?{DIGIT}+"."{DIGIT}* { yylval.fl = (float)atof(yytext); return(NUMBER);   }
extrude               { return(EXTRUDE);  }
expand                { return(EXPAND);   }
random                { return(RANDOM);   }
material              { return(MATERIAL);   }
subdivide             { return(SUBDIVIDE);}
{ID}                  { yylval.id = strdup(yytext); return(NONTERM);  }
#[^\n\r]*
[ \t\n\r]+
.                     { return(yytext[0]); }
%%

int yywrap () {
  return 1;
}

void yyerror(RuleSet *ruleset, char* s)
{
  printf ("%s at line %d\n", s, yylineno);
}
