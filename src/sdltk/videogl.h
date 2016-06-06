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
#ifndef SDLTK_VIDEOGL_H
#define SDLTK_VIDEOGL_H

#include "def.h"
#include "video.h"
#include "canvasgl.h"
#include "cpc.h"
#include "SDL.h"
#include "glfuncs.h"

namespace sdltk
{

    //! @author Fred Klaus development@fkweb.de
    class VideoGL : public Video
    {

    public:
        VideoGL(cpcx::Cpc * cpc);
        virtual ~VideoGL();

        virtual int init();
        virtual int init(uint width, uint height, uint depth, bool fullscreen, unsigned char scale);

        virtual Canvas * getCanvas() {return &mCanvas;}

        virtual void setup();
        virtual void update() {SDL_GL_SwapBuffers();}

        virtual void quit();
        virtual void lock()   {}
        virtual void unlock() {}

    private:
        GLuint mTexWidth;
        GLuint mTexHeight;
        GLuint mTexnum;

        uint*  calcScreenStart();
        uint*  calcScreenEnd();

        static volatile bool mIsLoaded;

        CanvasGL mCanvas;
    };

} // sdltk

#endif // SDLTK_VIDEOGL_H
