/***************************************************************************
 *   Copyright (C) 2005 by Fred Klaus                                      *
 *   frednet@web.de                                                        *
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
#ifndef CPC_FDC_H
#define CPC_FDC_H

#include "fdcconst.h"
#include "types.h"
#include "drive.h"
#include "cmdtable.h"

#include <cstdio>


namespace cpcx
{
    //! The floppy drive controller.
    class Fdc final
    {

    public:
        Fdc();
        ~Fdc() ROLAND_DEFAULT

        typedef void (Fdc::*CmdHandler)(void);

        void  write_data(tUBYTE val);
        tUBYTE read_status();
        tUBYTE read_data();
        void  specify();
        void  drvstat();
        void  recalib();
        void  intstat();
        void  seek();
        void  readtrk();
        void  write();
        void  read();
        void  readID();
        void  writeID();
        void  scan();

        void check_unit();
        int  init_status_regs();
        Sector* find_sector(tUBYTE *requested_CHRN);
        inline void cmd_write();
        inline void cmd_read();
        inline void cmd_readtrk();
        inline void cmd_scan();

        int  dsk_load(const char *pchFileName, int drv = 0, char chID='A');
        void dsk_eject(int drv = 0);

        //void scanlo();
        //void scanhi();

        int  motor()             {return mMotor;}
        void setMotor(int s)     {mMotor = s;}
        int  flags()             {return mFlags;}
        void addFlags(int flags) {mFlags |= flags;}
        int  phase()             {return mPhase;}
        int  timeout()           {return mTimeout;}
        void setTimeout(int val) {mTimeout = val;}

        int  cmdDirection()      {return mCmdDirection;}

        bool led()               {return mLed;}

    private:
        int  mTimeout;
        int  mMotor;
        bool mLed;
        int  mFlags;
        int  mPhase;
        int  mByteCount;
        int  mBufferCount;
        int  mCmdLength;
        int  mResLength;
        int  mCmdDirection;
        CmdHandler mCmdHandler;
        //void (Fdc::*mCmdHandler)(void);

        tUBYTE* pbGPBuffer;

        tUBYTE* mBufferPtr;
        tUBYTE* mBufferEndPtr;
        tUBYTE  mCommand[12];
        tUBYTE  mResult[8];

        Drive mDriveA;
        Drive mDriveB;

        Drive *mActiveDrive; // reference to the currently selected drive
        Track *mActiveTrack; // reference to the currently selected track, of the active_drive
        tUWORD mReadStatusDelay;
        tUWORD mBytesTransferred;

        CmdTable mCmdTable;

        FILE *pfileObject;

    };
} // namespace cpcx
#endif


