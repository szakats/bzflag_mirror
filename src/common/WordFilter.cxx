/* bzflag
 * Copyright (c) 1993 - 2003 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* no header other than WordFilter.h should be included here */
#include "WordFilter.h"


/* protected */

bool WordFilter::simpleFilter(char *input) const
{
  bool filtered = false;
  filter_t findWord;
  
  int randomCharPos=0;
  int previousCharPos=0;
  int maxFilterChar=filterChars.length();
  
  std::string line = input;
  int startPosition = line.find_first_of(alphabet);

  int endPosition;
  std::string word;

  /* here we iterate over all of the words in the input and replace
   * exact matches with asterisks
   */
  while (startPosition >= 0) {
    endPosition = line.find_first_not_of(alphabet, startPosition+1);
    if (endPosition < 0)
      endPosition = line.length();

    word = line.substr(startPosition, endPosition-startPosition);
    findWord.word = word;
    if (filters[word[0]].find(findWord) != \
	filters[word[0]].end()) {

      /* fill with asterisks */
      //      memset(input+startPosition,'*', endPosition-startPosition);
      
      /* fill with random filter chars */
      if (filterCharacters(input, startPosition, endPosition, true) > 0) {
	filtered=true;
      }
    }
    startPosition = line.find_first_of(alphabet, endPosition);
  }
  return filtered;
} // end simpleFilter


bool WordFilter::aggressiveFilter(char *input) const
{
  bool filtered = false;
  regex_t *compiledReg;
  static regmatch_t match[1];
  char errorBuffer[512];
  
  /* implicit limit of only match up to 128 words per input */
  /* !!! make dynamic */
  int matchPair[128 * 2];
  int matchCount = 0;
  
  int regCode;
  
  int randomCharPos;
  int previousCharPos;
  int maxFilterChar=filterChars.length();

  int startPosition = firstPrintable(input);
  if (startPosition < 0) {
    return false;
  }
  std::string line = input + startPosition;

  int inputPosition = startPosition;
  startPosition = 0;

  int endPosition;
  //  std::string word;
  
  char characterIndex;

  /* iterate over all the words start position in the input */
  while (startPosition >= 0) {
    endPosition = firstNonprintable(line);
    if (endPosition < 0) {
      endPosition = line.length();
    }
    //    word = line.substr(startPosition, endPosition-startPosition);

    // words are hashed by lowercase first letter
    characterIndex = tolower(line[startPosition]);

    for (std::set<filter_t, expressionCompare>::iterator i = filters[characterIndex].begin(); \
	 i != filters[characterIndex].end(); 
	 ++i) {
    
      regCode = regexec(i->compiled, input + inputPosition, 1, match, NULL);
      
      if ( regCode == 0 ) {
	//				std::cout << "Matched [" << i->word << "] with substring [" << input + match[0].rm_so << "] for " << match[0].rm_eo << " position; so was " << match[0].rm_so << std::endl;
      
	/* only consider matches from the beginning */
	if (match[0].rm_so == 0) {
	  matchPair[matchCount * 2] = match[0].rm_so + inputPosition; /* position */
	  matchPair[(matchCount * 2) + 1] = match[0].rm_eo - match[0].rm_so; /* length */
	  matchCount++;
	  filtered = true;
	}

      } else if ( regCode == REG_NOMATCH ) {
	
	// do nothing
	//			continue;
      } else {
	regerror(regCode, i->compiled, errorBuffer, 512);
	std::cout << errorBuffer << std::endl;
      }
    
    }

    // trim off the front of the line including the last non-printable
    line.erase(0, endPosition + 1);
    inputPosition += endPosition + 1;

    startPosition = firstPrintable(line); // should be zero most of the time
    if (startPosition > 0) {
      line.erase(0,startPosition);
      inputPosition += startPosition;
      startPosition = 0;
    }
  }

  
  /* finally filter the input.  only filter actual alphanumerics. */
  for (int i=0; i < matchCount; i++) {
    //			std::cout << "i: " << i << "  " << matchPair[i*2] << " for " << matchPair[(i*2)+1] << std::endl;
    
    if (filterCharacters(input, matchPair[i*2], matchPair[(i*2)+1]) <= 0) {
      // XXX throw exception
      std::cerr << "Unable to filter characters" << std::endl;
    }
  }
  
  
  return filtered;
} // end aggressiveFilter


// provides a pointer to a fresh compiled expression for some given expression
regex_t *WordFilter::getCompiledExpression(const std::string &word) const
{
  regex_t *compiledReg;
  
  /* XXX need to convert this to use new/delete */
  if ( (compiledReg = (regex_t *)calloc(1, sizeof(regex_t))) == NULL ) {
    
    perror("calloc failed");
    std::cerr << "Warning: unable to allocate memory for compiled regular expression";
    return (regex_t *)NULL;
    
  }
  
  // ??? need to test performance of REG_NOSUB until we get a match
  if ( regcomp(compiledReg, word.c_str(), REG_EXTENDED | REG_ICASE) != 0 ) {
    std::cerr << "Warning: unable to compile regular expression for [" << word << "]" << std::endl;
    return (regex_t *)NULL;
  }
  return compiledReg;
}


std::string WordFilter::expressionFromString(const std::string &word) const
{
  /* create the regular expression description */
  std::string expression;
  unsigned int length = word.length();
  char c[8];
  
  /* individual characters expand into a potential set of matchable characters */
  for (int i = 0; i < length; i++) {
    c[0] = c[1] = c[2] = c[3] = c[4] = c[5] = c[6] = c[7] = 0;
    // convert to lowercase for simplicity and speed
    c[0] = tolower(word[i]);
    
    /* we specifically will create a regular expression that should at least
     * match exactly the given input, including any spaces or special
     * characters.  including spaces or other characters in the input will
     * make them required to create a match.  BUT.. they need to be escaped
     * so that the regexp compiles properly.
     */
    if (( c[0] < 48 ) || ((c[0] > 57) && (c[0] < 65)) || ((c[0] > 90) && (c[0] < 97)) || (c[0] > 122)) {
      /* escape the non-alphanumeric */
      c[1] = c[0];
      c[0] = '\\';
    }
    /* character classes for l33t-speak */
    if ( c[0] == 'l' ) {
      c[1] = 'i';
      c[2] = '1';
      c[3] = '|';
      c[4] = '/';
      c[5] = '\\';
      //      c[6] = '\\';
    } else if ( c[0] == 'i' ) {
      c[1] = 'l';
      c[2] = '|';
      c[3] = '!';
      c[4] = '/';
      c[5] = '\\';
      //      c[6] = '\\';
    } else if ( c[0] == 'e' ) {
      c[1] = '3';
    } else if ( c[0] == 's' ) {
      c[1] = '$'; // should never be the last character
      c[2] = 'z';
      c[3] = '5';
    } else if ( c[0] == 't' ) {
      c[1] = '+';
    } else if ( c[0] == 'c' ) {
      c[1] = '\\';
      c[2] = '(';
    } else if ( c[0] == 'a' ) {
      c[1] = '4';
      c[2] = '@';
    } else if ( c[0] == 'b' ) {
      c[1] = '8';
    } else if ( c[0] == 'o' ) {
      c[1] = '0';
    } else if ( c[0] == 'g' ) {
      c[1] = '9';
    } else if ( c[0] == 'z' ) {
      c[1] = 's';
    }
    // need to handle 'f' special for f=>(f|ph)
    
    /* append multi-letter expansions */
    if (c[0] == 'f') {
      /* ensure we don't capture non-printables after end of word */
      if (i != length - 1) {
	expression.append("[fp]+[^[:alpha:]]*h?[^[:alpha:]]*");
      } else {
	expression.append("[fp]+h?");
      }
    } else {
      if ( c[1] != 0 ) {
	/* appends characters classes */
	expression.append("[");
	expression.append(c);
	expression.append("]");
      } else {
	/* append single characters */
	expression.append(c);
      }

      /* ensure we don't capture non-printables after end of word. these do
       * not get appended to the special "f" case.
       */
      if (i != length - 1) {
	expression.append("+[^[:alpha:]]*");
      } else {
	expression.append("+");
      }
      
    } // end test for multi-letter expansions
    
  } // end iteration over word letters
  
  //  std::cout << "[" <<  expression << "]" << std::endl;

  return expression;
}


/* public: */

WordFilter::WordFilter()
{
  filter_t fix;

  /* set up the alphabet for simple filtering */
  alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  /* filter characters randomly used to replace filtered text */
  filterChars = "!@#$%^&*";
  
  /* SUFFIXES */

  // noun
  fix.word = "dom";
  suffixes.insert(fix);
  fix.word = "ity";
  suffixes.insert(fix);
  fix.word = "ment";
  suffixes.insert(fix);
  fix.word = "sion";
  suffixes.insert(fix);
  fix.word = "tion";
  suffixes.insert(fix);
  fix.word = "ness";
  suffixes.insert(fix);
  fix.word = "ance";
  suffixes.insert(fix);
  fix.word = "ence";
  suffixes.insert(fix);
  fix.word = "er";
  suffixes.insert(fix);
  fix.word = "or";
  suffixes.insert(fix);
  fix.word = "ist";  
  suffixes.insert(fix);
  // adjective
  fix.word = "ive";
  suffixes.insert(fix);
  fix.word = "en";
  suffixes.insert(fix);
  fix.word = "ic";
  suffixes.insert(fix);
  fix.word = "al";
  suffixes.insert(fix);
  fix.word = "able";
  suffixes.insert(fix);
  fix.word = "y";
  suffixes.insert(fix);
  fix.word = "ous";
  suffixes.insert(fix);
  fix.word = "ful";
  suffixes.insert(fix);
  fix.word = "less";
  suffixes.insert(fix);
  // verb
  fix.word = "en";
  suffixes.insert(fix);
  fix.word = "ize";
  suffixes.insert(fix);
  fix.word = "ate";
  suffixes.insert(fix);
  fix.word = "ify";
  suffixes.insert(fix);
  fix.word = "fy";
  suffixes.insert(fix);
  fix.word = "ed";  
  suffixes.insert(fix);
  // adverb
  fix.word = "ly";  
  suffixes.insert(fix);
  // slang
  fix.word = "a";
  suffixes.insert(fix);
  fix.word = "z";
  suffixes.insert(fix);
  fix.word = "r";
  suffixes.insert(fix);
  fix.word = "ah";
  suffixes.insert(fix);
  fix.word = "io";
  suffixes.insert(fix);
  fix.word = "rs";
  suffixes.insert(fix);
  fix.word = "rz";
  suffixes.insert(fix);
  fix.word = "in";
  suffixes.insert(fix);
  fix.word = "n";
  suffixes.insert(fix);
  fix.word = "ster";
  suffixes.insert(fix);
  fix.word = "meister";
  suffixes.insert(fix);
  // plurality
  fix.word = "s";
  suffixes.insert(fix);
  fix.word = "es";
  suffixes.insert(fix);
  // imperfect verb
  fix.word = "ing";
  suffixes.insert(fix);
  // diminutive
  fix.word = "let";
  suffixes.insert(fix);

  for (std::set<filter_t, expressionCompare>::iterator i=suffixes.begin(); \
       i != suffixes.end(); ++i) {
    std::cout << "suffix:     " << i->word << std::endl;
    i->expression = this->expressionFromString(i->word);
    std::cout << "expression: " << i->expression << std::endl;
  }

  /* PREFIXES */

  // bz-specific

#if 0
  /* XXX adding prefixes significantly increases the expression count
   * and is rather expensive (slow)
   */
  fix.word = "bz";
  prefixes.insert(fix);
  fix.word = "bez";
  prefixes.insert(fix);
  fix.word = "beze";
  prefixes.insert(fix);
  
  for (std::set<filter_t, expressionCompare>::iterator i=prefixes.begin(); i != prefixes.end(); ++i) {
    std::cout << "suffix:     " << i->word << std::endl;
    i->expression = this->expressionFromString(i->word);
    std::cout << "expression: " << i->expression << std::endl;
  }
#endif

  return;
}

/** destructor releases the compiled bad words */
WordFilter::~WordFilter(void) 
{
  for (int j = 0; j < MAX_FILTERS; ++j) {
    for (std::set<filter_t, expressionCompare>::iterator i = filters[j].begin(); 
	 i != filters[j].end(); 
	 ++i) {
      free(i->compiled);
    }
  }

  return;
}
// consider calling regfree()


// adds an individual word to the filter list
bool WordFilter::addToFilter(const std::string &word, const std::string &expression, bool append)
{
  long int length = (long int)word.length();  
  if (0 >= length) {
    return false;
  } // end check if word is empty

  if (expression.size() == 0) {
    /* make sure to create an expression if it wasn't given */
    std::string expression = expressionFromString(word);
    addToFilter(word, expression, append);

  } else if (append) {
    filter_t appendWord;
    
    /* add words with all suffixes appended */
    for (std::set<filter_t, expressionCompare>::iterator i = suffixes.begin();
	 i != suffixes.end(); ++i) {
      addToFilter(word + i->word, expression + i->expression, false);
    }
    
    /* add words with all prefixes prepended */
    for (std::set<filter_t, expressionCompare>::iterator i = prefixes.begin();
	 i != prefixes.end(); ++i) {
      addToFilter(i->word + word, i->expression + expression, false);
      
      /* add words with prefix and suffix appended */
      for (std::set<filter_t, expressionCompare>::iterator j = suffixes.begin();
	   j != suffixes.end(); ++j) {
	addToFilter(i->word + word + j->word, 
		    i->expression + expression + j->expression, 
		    false);
      }
      
    }
  } else {
    /* base case */
    filter_t newFilter;
    
    newFilter.word = word;
    newFilter.expression = expression;
    newFilter.compiled = getCompiledExpression(expression);
    
    filters[tolower(newFilter.word[0])].insert(newFilter);
  }

  return true;
} // end addToFilter
bool WordFilter::addToFilter(const std::string &word, const std::string &expression)
{
  return addToFilter(word, expression, false);
}
bool WordFilter::addToFilter(const std::string &word, bool append)
{
  return addToFilter(word, "", append);
}
bool WordFilter::addToFilter(const std::string &word)
{
  return addToFilter(word, "", false);
}



/** loads a set of bad words from a specified file */
void WordFilter::loadFromFile(const std::string &fileName)
{
  char buffer[2048];
  std::ifstream filterStream(fileName.c_str());
  
  if (!filterStream) {
    std::cerr << "Warning: '" << fileName << "' bad word filter file not found" << std::endl;
    return;
  }
  
  while (filterStream.good()) {
    filterStream.getline(buffer,2048);
    
    std::string filterWord = buffer;
    
    int position = filterWord.find_first_not_of("\r\n\t ");
    
    // trim leading whitespace
    if (position > 0) {
      filterWord = filterWord.substr(position);
    }
    
    position = filterWord.find_first_of("#\r\n");
    
    // trim trailing comments
    if ((position >= 0) && (position < (int)filterWord.length())) {
      filterWord = filterWord.substr(0, position);
    }
    
    position = filterWord.find_last_not_of(" \t\n\r");
    // first whitespace is at next character position
    position += 1;
    
    // trim trailing whitespace
    if ((position >=0) && (position < (int)filterWord.length())) {
      filterWord = filterWord.substr(0, position);
    }
    
#if 0
    std::cout << "[[[" <<	 filterWord << "]]]" << std::endl;
#endif
    
    this->addToFilter(filterWord, "", true);
    
  } // end iteration over input file
  
} // end loadFromFile


/** filters an input message either a complex regular expression-based
 * pattern match (default) catching hundreds of variations per filter
 * word or using a simple exact word match technique (original).
 */
bool WordFilter::filter(char *input, bool simple) const
{
  if (simple) {
    std::cout << "NOT aggressive" << std::endl;
    return simpleFilter(input);
  }
  std::cout << "aggressive" << std::endl;
  return aggressiveFilter(input);
}

void WordFilter::outputFilter(void) const
{
  for (int i=0; i < MAX_FILTERS; ++i) {
    int count=0;
    for (std::set<filter_t, expressionCompare>::iterator j = filters[i].begin(); \
	 j != filters[i].end(); \
	 ++j) {
      std::cout << count++ << ": " << j->word << std::endl;
      std::cout << "    " << j->expression << std::endl;
    }
  }
  
}
void WordFilter::outputWords(void) const
{
  //		std::cout << "size of compiled set is " << () << std::endl;
  for (int i=0; i < MAX_FILTERS; ++i) {
    int count=0;
    for (std::set<filter_t, expressionCompare>::iterator j = filters[i].begin(); \
	 j != filters[i].end(); \
	 ++j) {
      std::cout << count++ << ": " << j->word << std::endl;
    }
  }
  
}
unsigned long int WordFilter::wordCount(void) const
{
  int count=0;
  for (int i=0; i < MAX_FILTERS; ++i) {
    for (std::set<filter_t, expressionCompare>::iterator j = filters[i].begin(); \
	 j != filters[i].end(); \
	 ++j) {
      count += 1;
    }
  }
  return count;
}


#if UNIT_TEST
int main (int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "missing filename" << std::endl;
    return -1;
  }
  
  WordFilter filter;
  //  filter.addToFilter("fuck?!", true);
  filter.addToFilter("test");
  //  filter.addToFilter("tah fei kei (tfk)?", true);

  filter.outputWords();
  filter.outputFilter();

  char message3[1024] = " This test is a fucKing simple test; you're NOT a beezeebitch!! ";
  std::cout << message3 << std::endl;
  filter.filter(message3, false);
  std::cout << message3 << std::endl;

  std::cout << "Loading file" << std::endl;
  filter.loadFromFile(argv[1]);
  std::cout << "Number of words in filter: " << filter.wordCount() << std::endl;
  filter.addToFilter("test", true);
  std::cout << "Number of words in filter: " << filter.wordCount() << std::endl;

  char message[1024] = " This test is a fucKing simple test; you're NOT a beezeebitch!! ";
  std::cout << message << std::endl;
  filter.filter(message, true);
  std::cout << message << std::endl;
  
  char message2[1024] = "f  u  c  k  !  fuuukking test ; you're NOT a beezeecun+y!!  Phuck you b1tch! ";
  std::cout << message2 << std::endl;
  filter.filter(message2);
  std::cout << message2 << std::endl;
  
  //  filter.outputWords();
  return 0;
}
#endif
// ex: shiftwidth=2 tabstop=8
