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

#ifndef __VOTINGBOOTH_H__
#define __VOTINGBOOTH_H__

#include <ctype.h>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

// work around an ugly STL bug in BeOS
// FIXME someone test whether it is still needed
#ifdef __BEOS__
#define private public
#endif
#include <bitset>
#ifdef __BEOS__
#undef private
#endif


/** VotingBooth is a means to create and track a vote.  A single booth
 * will track and allow voting on a poll.
 *
 * By default, false and true (i.e. no and yes) are the respective first
 * two default results of any poll if unspecified.
 */
class VotingBooth
{	
private:
  
  static const short int RETRACTED_VOTE;
  
  /** question that is voted upon (optionally provided)
   */
  std::string _question;

  /** vector of potential poll response choices
   */
  std::vector<std::string> _choice;

  /** collection of the voters and their response responses (index into the
   * choice vector)
   */
  std::map<std::string, short int> _vote;

protected:

 public:

  VotingBooth(std::string question = "");
  VotingBooth(const VotingBooth& booth);
  ~VotingBooth(void);

  /** add a response to vote upon.  vote responses are the choices that
   * may be voted upon (e.g. "yes", "no", "maybe", etc).
   */
  bool addResponse(const std::string response);

  /** return truthfully if a particular person has already voted
    */
  bool hasVoted(const std::string voterName) const;

  /** a given user id/name responds and votes to a particular poll
   * response.  returns truthfully whether the vote was placed.
   */
  bool vote(const std::string voterName, const std::string response);

  /** allow a vote to be retracted, returns truefully whether a retraction
   * was possible.
   */
  bool retractVote(const std::string voterName);

  /** return how many votes have been placed for a particular response.
   */
  unsigned long int getVoteCount(const std::string response) const;

  /** return total number of votes received
   */
  unsigned long int getTotalVotes(void) const;

  /** returns the number of voters that have participated
   */
  inline unsigned long int getVoterCount(void) const {
    return _vote.size();
  }

  /** returns the number of responses available
   */
  inline unsigned long int getResponseCount(void) const {
    return _choice.size();
  }

  /** returns a string identifier for this poll
   */
  inline const std::string getPollName(void) const {
    return _question;
  }

};

/* convenience func that sets up and returns a default boolean poll */
VotingBooth *YesNoVotingBooth(std::string question = "");

#else
class VotingBooth;
#endif

/* ex: shiftwidth=2 tabstop=8
 * Local Variables: ***
 * mode:C++ ***
 * tab-width: 8 ***
 * c-basic-offset: 2 ***
 * indent-tabs-mode: t ***
 * End: ***
 */
