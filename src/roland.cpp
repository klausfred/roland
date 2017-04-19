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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef USE_MMX
#include "mmx_memcpy.h"
#else
#include <memory>
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "videogl.h"
#include "videostd.h"
#include "gui.h"
#include "label.h"
#include "button.h"
#include "list.h"
#include "listitem.h"
#include "filelist.h"
#include "filelistitem.h"
#include "size.h"
#include "audio.h"
#include "clock.h"
#include "keytrans.h"
#include "color.h"
#include "events.h"

#include "cpc.h"
#include "prefs.h"
//#include "icon.h"

using namespace std;
using namespace cpcx;

uint framecount    = 0;
uint framecountsum = 0;
bool showfps  = false;
bool running  = true;
bool joystick = false;

void init();
void initGui();
void quit();
void clearKeyBuffer();
//void waitstates();
void mainloop();

//inline void update();
inline void display();

Prefs prefs;
Cpc cpcx::cpc(prefs);
sdltk::Audio    audio (&cpc);
sdltk::KeyTrans keytrans;
sdltk::Video    * video         = nullptr;
sdltk::Gui      * gui           = nullptr;
sdltk::Label    * lblFps        = nullptr;
sdltk::Label    * lblJoy        = nullptr;
sdltk::Label    * lblDisk       = nullptr;
sdltk::Label    * lblMenu       = nullptr;
sdltk::Label    * lblTitle      = nullptr;
sdltk::Button   * btnMenu       = nullptr;
sdltk::Button   * btnTest       = nullptr;
sdltk::Button   * btnTest2      = nullptr;
sdltk::Button   * btnDiskA      = nullptr;
sdltk::Button   * btnDiskB      = nullptr;
sdltk::List     * lstDirectory  = nullptr;
sdltk::FileList * lstFile       = nullptr;
sdltk::Clock mainClock;
sdltk::Clock displayClock;

static const string datadir(prefs.getPath("datadir"));

void init()
{
    cpc.init();
    showfps = prefs.getBool("showfps");

    cpc.fdc().dsk_eject(1);
    cpc.fdc().dsk_load(prefs.getPath("diskb").c_str(), 1);

    cpc.fdc().dsk_eject(0);
    cpc.fdc().dsk_load(prefs.getPath("diska").c_str(), 0);

    //video->init(0, 0, 0, prefs.getBool("fullscreen"));
    if (!video)
    {
        video = new sdltk::VideoStd(&cpc);
        if (video->init(0, 0, 0, false, 2) != 0)
        {
            delete video;
            EOUT("[SDLVidStd]", "could not init video device", "abort");
            quit();
        }
        initGui();
    }

    audio.init();
    mainClock.init();
    displayClock.init();

    clearKeyBuffer();
}

void initGui()
{
    if (gui) delete gui;
    gui = new sdltk::Gui(video);

    video->getCanvas()->setFont(
        datadir + "rpgfont.png",
        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;", 200);

    sdltk::Size textsize = video->getCanvas()->textSize("50/50");


    lblFps = new sdltk::Label;
    lblFps->setColor(100, 100, 100, 128);
    lblFps->setPos(7, video->screen()->h - textsize.height() - 10);
    lblFps->setSize(textsize.width() + 7, textsize.height() + 3);
    lblFps->setBorder(true);

    lblJoy = new sdltk::Label;
    lblJoy->setImage(datadir + "joykeyb.png");
    lblJoy->setPos(
            video->screen()->w - lblJoy->width()  - 5 - lblJoy->width(),
            video->screen()->h - lblJoy->height() - 5);
    lblJoy->setEnabled(joystick);

    lblDisk = new sdltk::Label;
    textsize = video->getCanvas()->textSize("disk");
    lblDisk->setSize(textsize.width() + 7, textsize.height() + 3);
    lblDisk->setText("disk");
    //lblDisk->setImage(datadir + "disk.png");
    lblDisk->setColor(64, 255, 64, 164);
    lblDisk->setPos(
            video->screen()->w - textsize.width()  - 14,
            video->screen()->h - textsize.height() - 10);
    lblDisk->setBorder(true);
    lblDisk->setEnabled(false);

    /*
    btnMenu = new sdltk::Button;
    btnMenu->setColor(100, 100, 100, 128);
    btnMenu->setImage(datadir + "menu2.png", false, true);
    btnMenu->setSize(32, 32);
    btnMenu->setPos(video->screen()->w - 32 - 10, 10);
    btnMenu->setWantEvents(true);
    btnMenu->setBorder(true);
    btnMenu->setEnabled(true);

    lblMenu = new sdltk::Label;
    lblMenu->setSize(video->screen()->w - 100, video->screen()->h - 100);
    lblMenu->setText("Menu");
    lblMenu->setColor(128, 128, 128, 160);
    lblMenu->setPos(50, 50);
    lblMenu->setBorder(true);
    lblMenu->setEnabled(false);

    lblTitle = new sdltk::Label(lblMenu);
    lblTitle->setSize(100, 20);
    lblTitle->setText("Menuxxxxx");
    lblTitle->setColor(128, 128, 0, 200);
    lblTitle->setPos(1, 1);
    lblTitle->setEnabled(true);*/
/*
    btnTest = new Button;
    btnTest->setPos(50, 50);
    btnTest->setSize(100, 100);
    btnTest->setEnabled(true);
    btnTest->setBorder(true);
    btnTest->setWantEvents(true);
    btnTest->setText("Knopf1");

    btnTest2 = new Button;
    btnTest2->setPos(110, 110);
    btnTest2->setSize(100, 100);
    btnTest2->setEnabled(true);
    btnTest2->setBorder(true);
    btnTest2->setWantEvents(true);
    btnTest2->setText("Knopf2");
*/

    btnDiskA = new sdltk::Button;
    btnDiskA->setPos(1, 1);
    btnDiskA->setSize(75, 20);
    btnDiskA->setColor(128, 128, 128, 164);
    btnDiskA->setEnabled(true);
    btnDiskA->setBorder(true);
    btnDiskA->setWantEvents(true);
    btnDiskA->setText("Disk A");

    btnDiskB = new sdltk::Button;
    btnDiskB->setPos(btnDiskA->width()+2, 1);
    btnDiskB->setSize(75, 20);
    btnDiskB->setColor(128, 128, 128, 164);
    btnDiskB->setEnabled(true);
    btnDiskB->setBorder(true);
    btnDiskB->setWantEvents(true);
    btnDiskB->setText("Disk B");


    lstFile = new sdltk::FileList(gui, prefs.getPath("diskdir"));
    lstFile->setEnabled(false);
    //lstFile->clear();

    gui->add(lblFps);
    gui->add(lblDisk);
    gui->add(lblJoy);
    gui->add(btnDiskA);
    gui->add(btnDiskB);
    //gui->add(btnMenu);
    //gui->add(lblMenu);
    //gui->add(lblTitle);
    //gui->add(btnTest);
    //gui->add(btnTest2);
    //gui->add(lstDirectory);

    gui->add(lstFile);
    //gui->add(lstFile);

    gui->setFocus(lstFile);
    gui->setEnabled(true);

}

void mainloop()
{
    SDL_Event event;
    tUBYTE cpc_key;
    static uint frametime =
        (int)((double)20.0 / (double)((cpc.speed() * 25) / 100.0));
    static uint cyclesElapsed = 0;
    static int ticksadjust = 0;
    // bool frameskip=false;

    // audio.pause(false);

    // Check key events
    while (running)
    {
        if (keytrans.hasSequence())
        {
            sdltk::KeyTrans::SeqPair sp = keytrans.sequenceVal();
            cpc_key = sp.keyval;
            if (sp.down)
                cpc.keyboard().setValue((cpc_key >> 4),
                                        (cpc.keyboard().value((cpc_key >> 4)) &
                                         ~bit_values[(cpc_key & 7)]));
            else
                cpc.keyboard().setValue((cpc_key >> 4),
                                        (cpc.keyboard().value((cpc_key >> 4)) |
                                         bit_values[(cpc_key & 7)]));
        }
        else
        {
//            if (btnMenu->isDown())
//            {
//                lblMenu->setEnabled(!lblMenu->enabled());
//                lblTitle->setEnabled(true);
//                btnMenu->setDown(false);
//            }
            static unsigned char drive = 0;
            if (btnDiskA->isDown())
            {
                drive = 0;
                lstFile->setEnabled(true);
            }
            if (btnDiskB->isDown())
            {
                drive = 1;
                lstFile->setEnabled(true);
            }
            while (SDL_PollEvent(&event) > 0)
            {
                gui->setFocus(lstFile);
                //if (gui->checkEvent(&event)) continue;
                gui->checkEvent(&event);
                switch (event.type)
                {
                    case SDL_USEREVENT:
                        if (event.user.code == sdltk::UserEvent::ListItemDoubleClicked)
                        {
                            IOUT("[roland.cpp]", "UserEvent::ListItemDoubleClicked", "received");
                            lstFile->setEnabled(false);
                            tSTRING str =
                                    prefs.getPath("diskdir")
                                    + ((sdltk::ListItem*) event.user.data1)->getText();
                            IOUT("[Roland]", "Disk Drive " + std::string(drive ? "B:" : "A:"), str);
                            cpc.fdc().dsk_eject(drive);
                            cpc.fdc().dsk_load(str.c_str(), drive);
                            prefs.set("diska", str);
                            SDL_EnableKeyRepeat(0, 0);
                        }
                        break;
                    case SDL_KEYDOWN:
                        // std::cout << "Key: " << event.key.keysym.sym << "\n";
                        cpc_key = keytrans.get(event);
                        if (!lstFile->enabled() && cpc_key != 0xff)
                        {
                            cpc.keyboard().setValue(
                                (cpc_key >> 4),
                                (cpc.keyboard().value((cpc_key >> 4)) &
                                 ~bit_values[(cpc_key & 7)]));
                        }
                        else
                        {
                            switch (static_cast<int>(event.key.keysym.sym))
                            {
                                case SDLK_RETURN:
                                    if (event.key.keysym.mod & KMOD_LALT)
                                    {
                                        audio.pause(true);
                                        video->toggleFullscreen();
                                        audio.pause(false);
                                        break;
                                    }
                                    if (lstFile->enabled() && gui->enabled())
                                    {
                                        lstFile->setEnabled(false);
                                        if (!lstFile->enabled() && !lstFile->empty())
                                        {
                                            tSTRING str =
                                                    prefs.getPath("diskdir")
                                                    + Prefs::delim()
                                                    + *lstFile->selected();
                                            IOUT("[Roland]", "Disk Drive A: ", str);
                                            cpc.fdc().dsk_eject(0);
                                            cpc.fdc().dsk_load(str.c_str(), 0);
                                            prefs.set("diska", str);
                                            SDL_EnableKeyRepeat(0, 0);
                                            clearKeyBuffer();
                                        }
                                        else SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
                                                                 SDL_DEFAULT_REPEAT_INTERVAL);
                                        break;
                                    }

                                case SDLK_F1:
                                {
                                    if (gui->enabled()) SDL_ShowCursor(0);
                                    else SDL_ShowCursor(1);
                                    gui->toggleEnabled();
                                    break;
                                }

                                case SDLK_F2:
                                case SDLK_F3:
                                    lstFile->setEnabled(!lstFile->enabled());
                                    if (lstFile->enabled() && lstFile->empty())
                                    {
                                        SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
                                                             SDL_DEFAULT_REPEAT_INTERVAL);
                                    }
                                    break;

                                    /* audio.pause(true);
                                    sdltk::FileSelect *f = new sdltk::FileSelect(
                                        video->screen(), prefs.getPath("diskdir"),
                                        prefs.getPath("diskb"), "B: ");
                                    SDL_EnableKeyRepeat(
                                        SDL_DEFAULT_REPEAT_DELAY,
                                        SDL_DEFAULT_REPEAT_INTERVAL);
                                    int ret = f->loop();
                                    SDL_EnableKeyRepeat(0, 0);
                                    if (!ret)
                                    {
                                        std::cout << "B: " << f->filename()
                                                  << "\n";
                                        cpc.fdc().dsk_eject(1);
                                        cpc.fdc().dsk_load(
                                            f->filename().c_str(), 1);
                                        prefs.set("diskb", f->filename());
                                    }
                                    clearKeyBuffer();
                                    audio.pause(false);
                                    delete f;
                                    break;*/

                                case SDLK_F4:
                                    joystick=keytrans.toggleJoystick();
                                    lblJoy->setEnabled(joystick);
                                    cout << "Joystick: " << boolalpha << joystick << "\n";
                                    break;

                                case SDLK_F5:
                                    audio.pause(true);
                                    video->toggleDoubling();
                                    audio.pause(false);
                                    break;

                                case SDLK_F6:
                                    audio.pause(true);
                                    video->toggleFilter();
                                    audio.pause(false);
                                    break;

                                case SDLK_F7:
                                    audio.pause(true);
                                    delete video;
                                    video = new sdltk::VideoStd(&cpc);
                                    if (video->init() != 0)
                                    {
                                        delete video;
                                        quit();
                                    }
                                    initGui();
                                    audio.pause(false);
                                    break;

                                case SDLK_F8:
                                    audio.pause(true);
                                    delete video;
                                    video = new sdltk::VideoGL(&cpc);
                                    if (video->init() != 0)
                                    {
                                        delete video;
                                        video = new sdltk::VideoStd(&cpc);
                                        if (video->init() != 0)
                                        {
                                            EOUT("[Core]", "could not init any video device", "quit");
                                            delete video;
                                            quit();
                                        }
                                        IOUT("[Core]", "fallback to VideoStd", "OK");
                                    }
                                    initGui();
                                    audio.pause(false);
                                    break;

                                case SDLK_F9:
                                    audio.pause(true);
                                    cpc.init();
                                    audio.pause(false);
                                    break;

                                case SDLK_F10:
                                    running=false;
                                    return;
                                    break;

                                case SDLK_F12:
                                    audio.pause(true);
                                    video->toggleFullscreen();
                                    SDL_ShowCursor(1);
                                    initGui();
                                    audio.pause(false);
                                    break;

                                default:
                                    break;
                            }
                        }
                        break;

                    case SDL_KEYUP:
                        cpc_key = keytrans.get(event);
                        if (cpc_key != 0xff)
                            cpc.keyboard().setValue(
                                (cpc_key >> 4),
                                (cpc.keyboard().value((cpc_key >> 4)) |
                                 bit_values[(cpc_key & 7)]));
                        break;

                    case SDL_QUIT: // We've been politely asked to exit, so
                                   // let's leave
                        running = false;
                        return;

                    default:
                        break;
                }
            }

        }
        if (audio.dist() < audio.mindist())
        {
            ticksadjust=-1; // speed emulation up to compensate @todo what to do here ???
            //cerr <<  "X";

        }
        else if (audio.dist() > audio.maxdist())
        {
            ticksadjust=1; // slow emulation down to compensate
            //SDL_Delay(1);
            //cerr <<  "O";
            continue;
            //cyclesElapsed=0;
        }



        /*
                while (mainClock.elapsed() < frametime+ticksadjust)
                {
                    SDL_Delay(1);
                }
        */

        // cerr << dwSndDist << " ";
        // if (ticksadjust > 0) {ticksadjust=0; continue;}

        // int t;
        // t=frametime-mainClock.elapsed();
        // cerr << t << " ";
        // if (mainClock.elapsed()<20) continue;
        // mainClock.init();

        // cyclesElapsed = cpc.z80().execute(CYCLE_COUNT_INIT);
        // cerr << cyclesElapsed  << " ";

        cyclesElapsed += cpc.z80().execute(CYCLE_COUNT_INIT);
        //cyclesElapsed = cpc.z80().execute(CYCLE_COUNT_INIT-cyclesElapsed);

        if (cpc.sound().bufferFull())
        {
            //while (!audio.copied()) {SDL_Delay(1);} // delay emulation
            //audio.setCopied(false);
            audio.waitCopied();
            cpc.sound().setBufferFull(false);
            continue;
        }

        if (cpc.vdu().frameCompleted()) // video emulation finished building frame?
        {
            cyclesElapsed=0;
            cpc.vdu().setFrameCompleted(false);

            //if (frameskip) {frameskip=false;mainClock.init();}
            //else {display();framecount++;}


            display();
            framecount++;

            cpc.vdu().setScrBase(video->bufferStart()); //@todo do not calculate if no pageflip or do in Video..
            //cpc.vdu().setScrEnd (video->buffer()End);//@todo do not calculate if no pageflip

            //if (mainClock.elapsed() > frametime) frameskip=true; // skip next frame
            if (ticksadjust >= 0) while (mainClock.elapsed()<frametime) {SDL_Delay(1);}
            //while (mainClock.elapsed()<frametime) {} @todo is this  OK ???
            mainClock.init();
        }
    }
}

inline void display()
{
    lblDisk->setEnabled(cpc.fdc().led());

    if (showfps)
    {
        if (displayClock.elapsed() >= 1000)
        {
            displayClock.init();
            framecountsum = framecount;
            framecount = 0;
        }

        string str = to_string(framecountsum);
        str += "/50";
        lblFps->setText(str);
    }

    video->setup();
    gui->update();
    video->update();
}

void clearKeyBuffer()
{
    SDL_Event event;
    cpc.keyboard().init();
    while (SDL_PollEvent(&event)) {};
}

void quit()
{
    audio.quit();
    SDL_Quit();
    IOUT("[Core]", "SDL", "cleanly finished");
}

int main(int argc, char *argv[])
{

    /* Initializes Audio and Video */
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        cerr << "[SDL] Could not init: " << SDL_GetError() << endl;
        SDL_Quit();
    }

#ifdef USE_MMX
    IOUT("[Core]", "MMX", "enabled");
#else
    IOUT("[Core]", "MMX", "disabled");
#endif

    //bool fs = prefs.getBool("fullscreen");

    //SDL_WM_SetCaption(PACKAGE_STRING, 0);
    sdltk::Video::setCaption(PACKAGE_STRING);

    /*SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*) ICON_DATA, 32, 32, 8, 32,
                                                 0xff0000, 0xff0000, 0xff0000, 0x000000);

    SDL_WM_SetIcon(surf, 0);
    */
    sdltk::Video::setIcon(datadir + "icon32x32-256.bmp");
    //SDL_WM_GrabInput (SDL_GRAB_ON);
    //SDL_ShowCursor (0);
    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_EnableKeyRepeat(0, 0);

    init();

    audio.pause(false);

    mainloop();

    quit();

    return 0;
}
