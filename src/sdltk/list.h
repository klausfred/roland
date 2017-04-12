/***************************************************************************
 *   Copyright (C) by Fred Klaus                                           *
 *       development@fkweb.de                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SDLTK_LIST_H
#define SDLTK_LIST_H

#include "listitem.h"
#include "gui.h"
#include "clock.h"
#include <list>

namespace sdltk
{

    class Color;
    class Point;

    //! A list that can hold several ListItem s.
    class List : public Widget, public std::list<ListItem*>
    {

    public:
        //! Some initialization
        List(Gui * gui) : mGui(gui) {init();}
        //! Deafault destructor
        virtual ~List() ROLAND_DEFAULT

        //! Init the List
        void init();

        //! This member catches mousebutton events
        virtual void onMouseButton(SDL_MouseButtonEvent * event);

        //! This member catches keyboard events
        virtual bool onKeyboard(SDL_KeyboardEvent * event);

        //! Tis member catches user events
        virtual bool onUser(SDL_UserEvent * event);

        //! Returns a pointer to the selected Item
        ListItem * selected() const {return (*mSelected);}

        //! Adds a ListItem to the List
        void add(ListItem * item);
        //! Manages the scrolling
        void reposition(Sint16 val);

        //! Draw the List and visible elements
        void draw();

    protected:
        Sint8  mSpeed;
        Uint8  mMotion;
        Uint16 mPosH;
        Gui *  mGui;
        Clock  mClock;
        Color  mTmpColor;
        std::list<ListItem*>::iterator mSelected;
    };

} // sdltk

#endif // SDLTK_LIST_H
