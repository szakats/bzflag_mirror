/* bzflag
 * Copyright (c) 1993 - 2009 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

// bzflag global header
#include "BackgroundTask.h"

#define BGTM_MAX_TICK 0.01f

// initialize the singleton
template <>
BackgroundTaskManager* Singleton<BackgroundTaskManager>::_instance = (BackgroundTaskManager*)0;


BackgroundTaskManager::BackgroundTaskManager()
{
  bz_registerEvent(bz_eTickEvent,this);
}

BackgroundTaskManager::~BackgroundTaskManager()
{
  bz_removeEvent(bz_eTickEvent,this);
}

void BackgroundTaskManager::addTask ( BackgroundTask *task, void *param )
{
  Task newTask;
  newTask.funcCB = NULL;
  newTask.classCB = task;
  newTask.param = param;
  tasks.push_back(newTask);
  bz_setMaxWaitTime(BGTM_MAX_TICK,"BackgroundTaskManager");
}

void BackgroundTaskManager::addTask ( BackgroundTaskFunc task, void *param )
{
  Task newTask;
  newTask.funcCB = task;
  newTask.classCB = NULL;
  newTask.param = param;
  tasks.push_back(newTask);
  bz_setMaxWaitTime(BGTM_MAX_TICK,"BackgroundTaskManager");
}

void BackgroundTaskManager::removeTask ( BackgroundTask *task, void *param )
{
  for (size_t i = 0; i < tasks.size(); i++ ) {
    if (tasks[i].classCB == task && tasks[i].param == param) {
      tasks.erase(tasks.begin()+i);
      return;
    }
  }
}

void BackgroundTaskManager::removeTask ( BackgroundTaskFunc task, void *param )
{
  for (size_t i = 0; i < tasks.size(); i++ ) {
    if (tasks[i].funcCB == task && tasks[i].param == param) {
      tasks.erase(tasks.begin()+i);
      return;
    }
  }
}

void BackgroundTaskManager::processTasks ( void )
{
  std::vector<size_t> itemsToKill;

  for (size_t i = 0; i < tasks.size(); i++ ) {
    Task &task = tasks[i];

    bool kill = false;
    if (task.funcCB)
      kill = (*task.funcCB)(task.param);
    else if (task.classCB)
      kill = task.classCB->process(task.param);
    else
      kill = true;

    if (kill)
      itemsToKill.push_back(i);
  }

  std::vector<size_t>::reverse_iterator itr = itemsToKill.rbegin();

  while ( itr != itemsToKill.rend() ) {
    tasks.erase(tasks.begin()+*itr);
    itr++;
  }
}

void BackgroundTaskManager::process ( bz_EventData *eventData )
{
  if (eventData->eventType != bz_eTickEvent)
    return; // WTF mate?

  processTasks();

  if ( tasks.size() )
    bz_setMaxWaitTime(BGTM_MAX_TICK,"BackgroundTaskManager");
  else
    bz_clearMaxWaitTime("BackgroundTaskManager");
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
