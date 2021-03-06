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

/*
 * HUDuiScrollList:
 *	User interface class for controls in a vertical scrollable list.
 *	Child controls can grab key presses and releases in order to
 *	maintain their functionality.
 */

#ifndef	__HUDUISCROLLLIST_H__
#define	__HUDUISCROLLLIST_H__

// ancestor class
#include "HUDuiNestedContainer.h"

#include "HUDuiLabel.h"

#include <string>
#include <list>

#include "BzfEvent.h"

class HUDuiScrollList : public HUDuiNestedContainer {
  public:
      HUDuiScrollList();
      HUDuiScrollList(bool paged);
      ~HUDuiScrollList();

    size_t getSelected() const;
    void setSelected(size_t _index);

    virtual void addItem(HUDuiControl* item);

    size_t getSize() { return items.size(); }

    HUDuiControl* get(size_t index);

    void update();
    void refreshNavQueue();
    void clear();

    void setSize(float width, float height);
    void setFontSize(float size);

    void setPaged(bool paged);

    static size_t callback(size_t oldFocus, size_t proposedFocus, HUDNavChangeMethod changeMethod, void* data);

  protected:
    bool doKeyPress(const BzfKeyEvent&);
    bool doKeyRelease(const BzfKeyEvent&);

    void resizeItems();

    void doRender();

    virtual size_t callbackHandler(size_t oldFocus, size_t proposedFocus, HUDNavChangeMethod changeMethod);

    std::list<HUDuiControl*> items;

  private:
    size_t index;
    size_t visiblePosition;
    size_t numVisibleItems;
    bool pagedList;

    HUDuiLabel* pageLabel;
};

#endif // __HUDUISCROLLLIST_H__

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
