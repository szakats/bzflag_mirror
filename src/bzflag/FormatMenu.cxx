/* bzflag
 * Copyright (c) 1993 - 2004 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* interface header */
#include "FormatMenu.h"

/* system implementation headers */
#include <vector>
#include <string>

/* common implementation headers */
#include "OpenGLTexFont.h"
#include "BzfWindow.h"
#include "BzfDisplay.h"

/* local implementation headers */
#include "MainMenu.h"

/* from playing.h */
BzfDisplay*		getDisplay();
bool			setVideoFormat(int, bool test);


const int		FormatMenu::NumReadouts = 4;
const int		FormatMenu::NumItems = 30;
const int		FormatMenu::NumColumns = 3;

bool FormatMenuDefaultKey::keyPress(const BzfKeyEvent& key)
{
  if (key.ascii == 0) switch (key.button) {
    case BzfKeyEvent::Up:
      if (HUDui::getFocus()) {
	menu->setSelected(menu->getSelected() - 1);
      }
      return true;

    case BzfKeyEvent::Down:
      if (HUDui::getFocus()) {
	menu->setSelected(menu->getSelected() + 1);
      }
      return true;

    case BzfKeyEvent::PageUp:
      if (HUDui::getFocus()) {
	menu->setSelected(menu->getSelected() - FormatMenu::NumItems);
      }
      return true;

    case BzfKeyEvent::PageDown:
      if (HUDui::getFocus()) {
	menu->setSelected(menu->getSelected() + FormatMenu::NumItems);
      }
      return true;
  }

  else if (key.ascii == '\t') {
    if (HUDui::getFocus()) {
      menu->setSelected(menu->getSelected() + 1);
    }
    return true;
  }

  else if (key.ascii == 'T' || key.ascii == 't') {
    menu->setFormat(true);
    return true;
  }
  return MenuDefaultKey::keyPress(key);
}

bool FormatMenuDefaultKey::keyRelease(const BzfKeyEvent& key)
{
  switch (key.button) {
    case BzfKeyEvent::Up:
    case BzfKeyEvent::Down:
    case BzfKeyEvent::PageUp:
    case BzfKeyEvent::PageDown:
      return true;
  }
  switch (key.ascii) {
    case 27:	// escape
    case 13:	// return
    case 'T':
    case 't':
      return true;
  }
  return MenuDefaultKey::keyRelease(key);
}

FormatMenu::FormatMenu() : defaultKey(this), badFormats(NULL)
{
  int i;

  BzfDisplay* display = getDisplay();
  numFormats = display->getNumResolutions();
  badFormats = new bool[numFormats];
  for (i = 0; i < numFormats; i++)
    badFormats[i] = false;

  // add controls
  addLabel("Video Format", "");
  addLabel("", "");			// instructions
  addLabel("", "Current Format:");	// current format readout
  addLabel("", "");			// page readout
  currentLabel = (HUDuiLabel*)(getControls()[NumReadouts - 2]);
  pageLabel = (HUDuiLabel*)(getControls()[NumReadouts - 1]);

  // add resolution list items
  for (i = 0; i < NumItems; ++i)
    addLabel("", "");

  // fill in static labels
  if (numFormats < 2) {
    currentLabel->setString("<switching not available>");
    setFocus(NULL);
  }
  else {
    HUDuiLabel* label = (HUDuiLabel*)(getControls()[NumReadouts - 3]);
    label->setString("Press Enter to select and T to test a format. Esc to exit.");
    setFocus(pageLabel);
  }
}

FormatMenu::~FormatMenu()
{
  delete[] badFormats;
}

void FormatMenu::addLabel(const char* msg, const char* _label)
{
  HUDuiLabel* label = new HUDuiLabel;
  label->setFont(MainMenu::getFont());
  label->setString(msg);
  label->setLabel(_label);
  getControls().push_back(label);
}

int FormatMenu::getSelected() const
{
  return selectedIndex;
}

void FormatMenu::setSelected(int index)
{
  BzfDisplay* display = getDisplay();
  std::vector<HUDuiControl*>& list = getControls();

  // clamp index
  if (index < 0)
    index = numFormats - 1;
  else if (index != 0 && index >= numFormats)
    index = 0;

  // ignore if no change
  if (selectedIndex == index)
    return;

  // update current format
  currentLabel->setString(display->getResolution(display->getResolution())->name);

  // update selected index and get old and new page numbers
  const int oldPage = (selectedIndex < 0) ? -1 : (selectedIndex / NumItems);
  selectedIndex = index;
  const int newPage = (selectedIndex / NumItems);

  // if page changed then load items for this page
  if (oldPage != newPage) {
    // fill items
    const int base = newPage * NumItems;
    for (int i = 0; i < NumItems; ++i) {
      HUDuiLabel* label = (HUDuiLabel*)list[i + NumReadouts];
      if (base + i < numFormats)
	if (badFormats[base + i])
	  label->setString("<unloadable>");
	else
	  label->setString(display->getResolution(base + i)->name);
      else
	label->setString("");
    }

    // change page label
    if (numFormats > NumItems) {
      char msg[50];
      std::vector<std::string> args;
      sprintf(msg, "%d", newPage + 1);
      args.push_back(msg);
      sprintf(msg, "%d", (numFormats + NumItems - 1) / NumItems);
      args.push_back(msg);
      pageLabel->setString("Page {1} of {2}", &args);
    }
  }

  // set focus to selected item
  if (numFormats > 0) {
    const int indexOnPage = selectedIndex % NumItems;
    getControls()[NumReadouts + indexOnPage]->setFocus();
  }
  else {
    setFocus(NULL);
  }
}

void FormatMenu::show()
{
  pageLabel->setString("");
  selectedIndex = -1;
  setSelected(getDisplay()->getResolution());
}

void FormatMenu::execute()
{
  setFormat(false);
}

void FormatMenu::setFormat(bool test)
{
  if (selectedIndex >= numFormats || badFormats[selectedIndex])
    return;

  if (!setVideoFormat(selectedIndex, test)) {
    // can't load format
    badFormats[selectedIndex] = true;
  }
  else if (!test) {
    // print OpenGL renderer, which might have changed
    printError((const char*)glGetString(GL_RENDERER));
  }

  // update readouts
  const int oldSelectedIndex = selectedIndex;
  selectedIndex = -1;
  setSelected(oldSelectedIndex);
}

void FormatMenu::resize(int width, int height)
{
  HUDDialog::resize(width, height);

  // use a big font for title, smaller font for the rest
  const float titleFontWidth = (float)height / 10.0f;
  const float titleFontHeight = (float)height / 10.0f;

  // reposition title
  float x, y;
  std::vector<HUDuiControl*>& list = getControls();
  {
    HUDuiLabel* title = (HUDuiLabel*)list[0];
    title->setFontSize(titleFontWidth, titleFontHeight);
    const OpenGLTexFont& titleFont = title->getFont();
    const float titleWidth = titleFont.getWidth(title->getString());
    x = 0.5f * ((float)width - titleWidth);
    y = (float)height - titleFont.getHeight();
    title->setPosition(x, y);
  }

  // reposition test and current format messages
  float fontWidth = (float)height / 36.0f;
  float fontHeight = (float)height / 36.0f;
  {
    HUDuiLabel* label = (HUDuiLabel*)list[1];
    label->setFontSize(fontWidth, fontHeight);
    const OpenGLTexFont& font = label->getFont();
    const float stringWidth = font.getWidth(label->getString());
    x = 0.5f * ((float)width - stringWidth);
    y -= 1.5f * font.getHeight();
    label->setPosition(x, y);
  }
  {
    HUDuiLabel* label = currentLabel;
    label->setFontSize(fontWidth, fontHeight);
    const OpenGLTexFont& font = label->getFont();
    y -= 1.0f * font.getHeight();
    label->setPosition(0.5f * (float)width, y);
  }

  // position page readout
  fontWidth = (float)height / 36.0f;
  fontHeight = (float)height / 36.0f;
  {
    HUDuiLabel* label = pageLabel;
    label->setFontSize(fontWidth, fontHeight);
    const OpenGLTexFont& font = label->getFont();
    const float stringWidth = font.getWidth(label->getString());
    x = 0.5f * ((float)width - stringWidth);
    y -= 2.0f * font.getHeight();
    label->setPosition(x, y);
  }

  // position format item list
  const float yBase = y;
  int lastColumn = -1;
  for (int i = 0; i < NumItems; ++i) {
    const int column = i * NumColumns / NumItems;
    if (column != lastColumn) {
      lastColumn = column;
      x = (float)width * ((0.5f + (float)column) / (float)(NumColumns + 1));
      y = yBase;
    }

    HUDuiLabel* label = (HUDuiLabel*)list[i + NumReadouts];
    label->setFontSize(fontWidth, fontHeight);
    const OpenGLTexFont& font = label->getFont();
    y -= 1.0f * font.getHeight();
    label->setPosition(x, y);
  }
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
