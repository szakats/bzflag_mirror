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

/* no header other than VotingBooth.h should be included here */

#ifdef _WIN32
#pragma warning( 4:4786)
#endif

#include "VotingBooth.h"


/* private */

/* protected */

/* public: */

VotingBooth::VotingBooth(std::string question, bool requireUnique) 
  : _question(question),
    _responseCount(0),
    _voterCount(0),
    _requireUnique(requireUnique)
{
  return;
}

VotingBooth::~VotingBooth(void) 
{
  return;
}

vote_t VotingBooth::addResponse(const std::string response)
{
  if (response.size() == 0) {
    return -1;
  }
  /* make sure there is enough room for the new response */
  if (_responseCount + 1 >= MAX_VOTE_RESPONSES) {
    return -1;
  }

  for (vote_t i=0; i < _responseCount; i++) {
    if (this->compare_nocase(_response[i], response, _response[i].size()) == 0) {
      return i;
    }
  }
  _response[_responseCount] = response;

  _responseCount++;

  return _responseCount - 1;
}

vote_t VotingBooth::getResponseIDFromString(const std::string response) const
{
  if (response.size() == 0) {
    return -1;
  }
  for (vote_t id = 0; id < _responseCount; id++) {
    if (VotingBooth::compare_nocase(_response[id], response, _response[id].size()) == 0) {
      return id;
    }
  }

  return -1;
}

const std::string VotingBooth::getStringFromResponseID(vote_t id) const
{
  if ((id >= 0) && (id < _responseCount)) {
    return _response[id];
  }

  return "";
}

bool VotingBooth::vote(std::string voter, vote_t id)
{
  /* if we are requiring unique, make sure the name does not exist */
  if (_requireUnique) {
    for (int i = 0; i < _voterCount; i++) {
      if (VotingBooth::compare_nocase(_voter[i], voter, voter.size()) == 0) {
	/* repeat voters are not allowed to vote */
	return false;
      }
    }
  }

  /* make sure the vote id is valid */
  std::string response = this->getStringFromResponseID(id);
  if (response.size() == 0) {
    return false;
  }

  /* add the voter to the list of voters */
  _voter[_voterCount] = voter;
  _voterCount++;

  /* finally increment the dang vote */
  _vote[id]++;

  return true;
}

unsigned long int VotingBooth::getVoteCount(vote_t id) const {
  return _vote[id];
}

unsigned long int VotingBooth::getVoteCount(const std::string response) const {
  if (response.size() == 0) {
    return 0;
  }
  for (vote_t id = 0; id < _responseCount; id++) {
    if (VotingBooth::compare_nocase(_response[id], response, _response[id].size()) == 0) {
      return this->getVoteCount(id);
    }
  }

  return 0;
}

unsigned long int VotingBooth::getTotalVotes(void) const {
  unsigned long int total=0;
  for (vote_t id = 0; id < _responseCount; id++) {
    total += this->getVoteCount(id);
  }
  return total;
}


#if UNIT_TEST
int main (int argc, char *argv[])
{
  VotingBooth *poll = getYesNoVotingBooth();

  poll->vote("blah1", 0);
  poll->vote("blah2", 0);
  poll->vote("blah3", 0);
  poll->vote("blah1", 1);
  poll->vote("blah2", 0);
  poll->vote("blah3", 1);
  poll->vote("blah1", 2);
  poll->vote("blah2", 3);
  vote_t newResponse = poll->addResponse("maybe");
  std::cout << "new response maybe has id: " << newResponse << std::endl;
  poll->vote("blah", 2);
  poll->vote("blah", 3);

  std::cout << "responseID for no is " << poll->getResponseIDFromString("no") << std::endl;
  std::cout << "responseID for yes is " << poll->getResponseIDFromString("yes") << std::endl;
  std::cout << "responseID for maybe is " << poll->getResponseIDFromString("maybe") << std::endl;
  
  std::string *response;
  for (int i=0; i < 9; i++) {
    std::cout << "response #" << i << " is " << poll->getStringFromResponseID(i) << std::endl;    
    std::cout << "  vote count is " << poll->getVoteCount(i) << std::endl;    
  }

  std::cout << "total votes is " << poll->getTotalVotes() << std::endl;

  return 0;
}
#endif
// ex: shiftwidth=2 tabstop=8
