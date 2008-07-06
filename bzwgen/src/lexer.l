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
-?{DIGIT}+            { yylval.fl = (double)atof(yytext); return(NUMBER);   }
-?{DIGIT}+"."{DIGIT}* { yylval.fl = (double)atof(yytext); return(NUMBER);   }
{DIGIT}+r             { yylval.fl = -(double)atof(yytext); return(NUMBER);   }
{DIGIT}+"."{DIGIT}*r  { yylval.fl = -(double)atof(yytext); return(NUMBER);   }
extrude               { return(EXTRUDE);  }
extrudet              { return(EXTRUDET);  }
expand                { return(EXPAND);   }
chamfer               { return(CHAMFER);   }
unchamfer             { return(UNCHAMFER);   }
random                { return(RANDOM);   }
assign                { return(ASSIGN);   }
scale                 { return(SCALE);   }
translate             { return(TRANSLATE); }
translater            { return(TRANSLATER); }
material              { return(MATERIAL);   }
loadmaterial          { return(LOADMATERIAL);   }
loadmaterialnr        { return(LOADMATERIALNR);   }
multiface             { return(MULTIFACE); }
repeath				  { return(REPEATH);}
repeatv               { return(REPEATV);}
splith				  { return(SPLITH);}
splitv                { return(SPLITV);}
face                  { return(FACE); }
addface               { return(ADDFACE); }
detachface            { return(DETACHFACE); }
free                  { return(FREE); }
ngon                  { return(NGON); }
drivethrough          { return(DRIVETHROUGH); }
texture               { return(TEXTURE); }
texturefull           { return(TEXTUREFULL); }
texturequad           { return(TEXTUREQUAD); }
textureclear          { return(TEXTURECLEAR); }
assert                { return(ASSERTION); }
spawn                 { return(SPAWN);}
remove                { return(REMOVE);}
taper                 { return(TAPER);}
neg                   { return(NEG);}
round                 { return(ROUND); }
"$"{ID}               { yylval.id = strdup(yytext+1); return(ATTRIBUTE);  }
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
