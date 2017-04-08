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
#ifndef CPC_CMDTABLE_H
#define CPC_CMDTABLE_H

#include "cmdtabledef.h"
#include "fdcconst.h"

namespace cpcx
{
    //! Command table
    class CmdTable final
    {
    public:
        CmdTable()  ROLAND_DEFAULT
        ~CmdTable() ROLAND_DEFAULT

        CmdTableDef & get(int num) {return entry[num];}

    private:
        CmdTableDef entry[MAX_CMD_COUNT];
    };
} // namespace cpcx

#endif // CPC_CMDTABLE_H
