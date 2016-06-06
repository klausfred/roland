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
#ifndef SDLTK_AUDIO_H
#define SDLTK_AUDIO_H

#include "def.h"
#include "cpc.h"
#include "SDL.h"


namespace sdltk
{

    //! Manages all Audio related stuff
    class Audio final
    {

    public:
        Audio(cpcx::Cpc * cpc);
        ~Audio();

        int  init();
        void quit();

        uint    dist() const;
        uint mindist() const  {return mMinSafeDist;}
        uint maxdist() const  {return mMaxSafeDist;}

        bool copied() const      {return mBufferCopied;}
        void setCopied(bool val) {mBufferCopied = val;}
        void waitCopied() {while (!mBufferCopied) {SDL_Delay(1);} mBufferCopied = false;}

        void pause(bool value);

        static void update(void *userdata, Uint8 *stream, int len);

    private:
        static cpcx::Cpc * mCpc;
        static volatile bool mBufferCopied; ///@todo not threadsafe
        SDL_AudioSpec* mSpec;

        bool isInit;

        uint mMinSafeDist;
        uint mMaxSafeDist;

        uint alignSamples(uint given);
    };

    inline uint Audio::dist() const // determine distance between play and write cursors
    {
        if (mCpc->sound().stream() < mCpc->sound().bufferPtr())
        {
            return mCpc->sound().bufferPtr()-mCpc->sound().stream();
        }
        else
        {
            return (mCpc->sound().bufferEnd() -  mCpc->sound().stream())
                    +
                   (mCpc->sound().bufferPtr() -  mCpc->sound().buffer());
        }
    }

} // sdltk

#endif // SDLTK_AUDIO_H
