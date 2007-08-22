%{
#include <memory>
#include <string>
#include "RuleSet.h"
#include "Product.h"
#include "Operation.h"
#include "Expression.h"
#include "parser.hxx"

// Ugly hacks to surpress warnings
#ifdef _MSC_VER
  #define strdup _strdup
#endif
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
ID [a-zA-Z_][a-zA-Z0-9_]*
%option yylineno
%%
"->"                  { return(DEFSIGN);  }
-?{DIGIT}+            { yylval.fl = (float)atof(yytext); return(NUMBER);   }
-?{DIGIT}+"."{DIGIT}* { yylval.fl = (float)atof(yytext); return(NUMBER);   }
extrude               { return(EXTRUDE);  }
expand                { return(EXPAND);   }
chamfer               { return(CHAMFER);   }
unchamfer             { return(UNCHAMFER);   }
random                { return(RANDOM);   }
assign                { return(ASSIGN);   }
material              { return(MATERIAL);   }
subdivideh            { return(SUBDIVIDEH);}
subdividev            { return(SUBDIVIDEV);}
partitionh            { return(PARTITIONH);}
partitionv            { return(PARTITIONV);}
partitionhi           { return(PARTITIONHI);}
partitionvi           { return(PARTITIONVI);}
face                  { return(FACE); }
texturefull           { return(TEXTUREFULL); }
assert                { return(ASSERTION); }
spawn                 { return(SPAWN);}
taper                 { return(TAPER);}
neg                   { return(NEG);}
"$"{ID}               { yylval.id = strdup(yytext+1); return(ATTRIBUTE);  }
{ID}                  { yylval.id = strdup(yytext); return(NONTERM);  }
#[^\n]*
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
