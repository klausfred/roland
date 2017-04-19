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
#include "listitem.h"
#include "events.h"
#include "clock.h"
#include "SDL.h"

namespace sdltk
{

    ListItem::ListItem()
    {
        mOrigin.set(0, 0);
        mEnabled = true;
        //mWantEvents = false;
        setColor(100, 100, 100, 128);
        setHighlightColor(Color(100, 100, 100, 160));
    }

    void ListItem::onMouseButton(SDL_MouseButtonEvent * event)
    {
        if (!parent()->enabled()) return;

        static sdltk::Clock doubleclick;

        if ((event->type == SDL_MOUSEBUTTONDOWN) && (event->button == SDL_BUTTON_LEFT))
        {
            if (doubleclick.elapsed() < 250 && doubleclick.elapsed() > 50)
            {
                SDL_Event event;
                event.type = SDL_USEREVENT;
                event.user.code  =  UserEvent::ListItemDoubleClicked;
                event.user.data1 = this;
                event.user.data2 = 0;
                SDL_PushEvent(&event);

                IOUT("[ListItem]", "UserEvent::ListItemDoubleClicked sent", getText());
                mDown = true;
                return;
            }
            else doubleclick.init();

            SDL_Event event;
            event.type = SDL_USEREVENT;
            event.user.code  =  UserEvent::ListItemClicked;
            event.user.data1 = this;
            event.user.data2 = 0;
            SDL_PushEvent(&event);

            IOUT("[ListItem]", "UserEvent::ListItemClicked sent", getText());
            mDown = true;
        }

        if ((event->type == SDL_MOUSEBUTTONUP) && (event->button == SDL_BUTTON_LEFT))
        {

            mDown = false;
        }

        moveInit(event);
    }

}
