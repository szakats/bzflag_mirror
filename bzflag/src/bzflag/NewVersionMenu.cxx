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

/* interface header */
#include "NewVersionMenu.h"

/* system headers */
#include <stdio.h>
#ifdef WIN32
#include <direct.h>
#include <process.h>
#include <errno.h>
#endif

/* common implementation headers */
#include "FontManager.h"
#include "StateDatabase.h"
#include "DirectoryNames.h"
#include "TextUtils.h"

/* local implementation headers */
#include "FontSizer.h"
#include "MainMenu.h"
#include "HUDDialogStack.h"
#include "HUDuiLabel.h"
#include "CommandsStandard.h"
#include "LocalFontFace.h"

/* as platforms get added to the automatic upgrade system, update this */
#if defined(WIN32)
  #define AUTOUPGRADE 1
#else
  #undef AUTOUPGRADE
#endif

NewVersionMenu::NewVersionMenu(std::string announce, std::string url, std::string date) :
cURLManager(), byteTransferred(0)
{
  // prep for possible download
  setURL(url);
#ifdef AUTOUPGRADE
  int timeout = 15;
  if (BZDB.isSet("httpTimeout")) {
    timeout = BZDB.evalInt("httpTimeout");
  }
  setTimeout(timeout);
#endif

  // add controls
  HUDuiLabel* label;
  const LocalFontFace* fontFace = MainMenu::getFontFace();

  status = label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString("");
  addControl(status, false);

  label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString(announce);
  addControl(label, false);

  label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString("A new version of BZFlag has been released!");
  addControl(label, false);

  label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString(date);
  addControl(label, false);

#ifdef AUTOUPGRADE
  label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString("Would you like to upgrade now?");
  addControl(label, false);

  label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString("(Download and install: " + url + ")");
  addControl(label, false);

  yes = label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString("Yes!");
  addControl(label);

  no = label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString("Not yet");
  addControl(label);

  initNavigation();
#else
  label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString("Please upgrade as soon as possible.");
  addControl(label, false);

  yes = NULL;

  no = label = new HUDuiLabel;
  label->setFontFace(fontFace);
  label->setString("OK");
  addControl(label);

  initNavigation();
#endif

}

NewVersionMenu::~NewVersionMenu()
{
}

void NewVersionMenu::execute()
{
  HUDuiControl* _focus = getNav().get();
  if (!_focus) {
    return;
#ifdef AUTOUPGRADE
  } else if (_focus == yes) {
    addHandle();
#endif
  } else if (_focus == no) {
    HUDDialogStack::get()->pop();
  }
}

void NewVersionMenu::collectData(char* ptr, int len)
{
  char buffer[128];
  double size = 0;
  getFileSize(size);
  cURLManager::collectData(ptr, len);
  byteTransferred += len;
  snprintf(buffer, 128, "Downloading update: %d/%d KB", byteTransferred/1024, (int)size/1024);
  ((HUDuiLabel*)status)->setString(buffer);
}

void NewVersionMenu::finalization(char *data, unsigned int length, bool good)
{
  if (good && data) {
    if (length) {
      // received update.  Now what to do with it?
#ifdef WIN32
      // make sure the directory exists
      mkdir(getTempDirName().c_str());
      // write data to temporary file
      const std::string tempfile = getTempDirName() + "\\temp-upgrade.exe";
      FILE* temp = fopen(tempfile.c_str(), "wb");
      if (temp)
	fwrite(data, 1, length, temp);
      fclose(temp);
      // start the program
      char* args [2];
      args[0] = "temp-upgrade.exe";
      args[1] = NULL;
      const int result = (const int)_spawnvp(_P_DETACH, tempfile.c_str(), args);
      if (result < 0) {
	if (errno == ENOENT)
	  ((HUDuiLabel*)status)->setString("Failed... can't find upgrade installer.");
	else if (errno == ENOMEM)
	  ((HUDuiLabel*)status)->setString("Failed... not enough memory.");
	else if (errno == ENOEXEC)
	  ((HUDuiLabel*)status)->setString("Failed... installer is not executable.");
	else
	  ((HUDuiLabel*)status)->setString(TextUtils::format("Failed... unknown error (%d).", errno).c_str());
	logDebugMessage(1,"Failed to start upgrade installer (%s) - error %d.\n", tempfile, errno);
      } else {
	((HUDuiLabel*)status)->setString("Installer started.");
	CommandsStandard::quit();
      }
#endif
    }
  } else {
    status->setLabel("Download Failed!");
  }
}

void NewVersionMenu::resize(int _width, int _height)
{
  HUDDialog::resize(_width, _height);
  FontSizer fs = FontSizer(_width, _height);

  FontManager &fm = FontManager::instance();
  const LocalFontFace* fontFace = MainMenu::getFontFace();

  // use a big font
  fs.setMin(0, (int)(1.0 / BZDB.eval("headerFontSize") / 2.0));
  float fontSize = fs.getFontSize(fontFace->getFMFace(), "headerFontSize");

  fs.setMin(0, 20);
  float smallFontSize = fs.getFontSize(fontFace->getFMFace(), "menuFontSize");
  float x, y;

  // heights
  const float fontHeight = fm.getStringHeight(fontFace->getFMFace(), fontSize);
  const float smallFontHeight = fm.getStringHeight(fontFace->getFMFace(), smallFontSize);

  // get stuff
  std::vector<HUDuiElement*>& listHUD = getElements();
  int i = 0;

  const int faceID = fontFace->getFMFace();

  // status
  HUDuiLabel* label = (HUDuiLabel*)listHUD[i];
  label->setFontSize(smallFontSize);
  float labelWidth = fm.getStringWidth(faceID, smallFontSize, "Downloading update: 8888/8888 KB");
  x = 0.5f * ((float)_width - labelWidth);
  y = 2.0f * fontHeight;
  label->setPosition(x, y);

  // announcement
  label = (HUDuiLabel*)listHUD[++i];
  label->setFontSize(fontSize);
  labelWidth = fm.getStringWidth(faceID, fontSize, label->getString());
  x = 0.5f * ((float)_width - labelWidth);
  y = (float)_height - fontHeight - 1.5f * smallFontHeight;
  label->setPosition(x, y);

  // release-notice
  label = (HUDuiLabel*)listHUD[++i];
  label->setFontSize(smallFontSize);
  labelWidth = fm.getStringWidth(faceID, smallFontSize, label->getString());
  x = 0.5f * ((float)_width - labelWidth);
  y -= 3.0f * smallFontHeight;
  label->setPosition(x, y);

  // release-date
  label = (HUDuiLabel*)listHUD[++i];
  label->setFontSize(smallFontSize);
  labelWidth = fm.getStringWidth(faceID, smallFontSize, label->getString());
  x = 0.5f * ((float)_width - labelWidth);
  y -= 1.5f * smallFontHeight;
  label->setPosition(x, y);

  // user request
  label = (HUDuiLabel*)listHUD[++i];
  label->setFontSize(smallFontSize);
  labelWidth = fm.getStringWidth(faceID, smallFontSize, label->getString());
  x = 0.5f * ((float)_width - labelWidth);
  y -= 4.5f * smallFontHeight;
  label->setPosition(x, y);

#ifdef AUTOUPGRADE
  // download details
  label = (HUDuiLabel*)listHUD[++i];
  label->setFontSize(smallFontSize);
  labelWidth = fm.getStringWidth(faceID, smallFontSize, label->getString());
  x = 0.5f * ((float)_width - labelWidth);
  y -= 1.5f * smallFontHeight;
  label->setPosition(x, y);
#endif

  // first user option
  label = (HUDuiLabel*)listHUD[++i];
  label->setFontSize(smallFontSize);
  labelWidth = fm.getStringWidth(faceID, smallFontSize, label->getString());
  x = 0.5f * ((float)_width - labelWidth);
  y -= 1.0f * fontHeight;
  label->setPosition(x, y);

#ifdef AUTOUPGRADE
  // second user option
  label = (HUDuiLabel*)listHUD[++i];
  label->setFontSize(smallFontSize);
  labelWidth = fm.getStringWidth(faceID, smallFontSize, label->getString());
  y -= 1.5f * smallFontHeight;
  label->setPosition(x, y);
#endif

}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
