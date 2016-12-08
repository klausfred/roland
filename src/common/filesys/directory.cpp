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
#include "directory.h"

#include <sys/types.h>
#if defined(_WIN32) || defined(_WIN64)
    #include "windirent.h"
#else
	#include <dirent.h>
#endif
#include <cstddef>

#include <algorithm>
#include <iostream>

using std::vector;

Directory::Directory(const FileName & path, Options options, char letter)
    : vector<FileName>()
{
    scan(path, options, letter);
}

void Directory::scan(const FileName &path, Options options, char letter)
{
    mDirName = path;
    clear();

    DIR *dir;
    dir = opendir(path.c_str());
    if (dir == 0)
        return; /** @todo errorhandling !! */

    struct dirent *entry;
#if defined(_WIN32) || defined(_WIN64)
    while (
        (entry = readdir(
             dir))) /**  @todo not threadsafe but readdir_r not in MinGW !! */
    {
        FileName fi(entry->d_name);
        if ((options & Options::ParentDir) && (fi == "..")) continue;
        if ((options & Options::OwnDir)    && (fi == "." )) continue;
        if (letter && tolower(fi.base()[0]) == tolower(letter)) push_back(fi);
        else if (!letter) push_back(fi);
    }
    free(entry); /** @todo this is a static portion of memory, need change ?? */
#else
    /** @todo cleanup */
    // following code is to ensure d_name is large enough.
    union
    {
        struct dirent d;
        char b[offsetof(struct dirent, d_name) + NAME_MAX + 1];
    } u;

    while (!readdir_r(dir, &u.d, &entry))
    {
        if (!entry) break;
        FileName fi(u.d.d_name);
        if ((options & Options::ParentDir) && (fi == "..")) continue;
        if ((options & Options::OwnDir)    && (fi == "." )) continue;
        if (letter && tolower(fi.base()[0]) == tolower(letter)) push_back(fi);
        if (!letter) push_back(fi);
    }
#endif

    closedir(dir);
}

void Directory::sort() {std::sort(begin(), end());}

void Directory::sort(bool casesensitive)
{
    FileName::setCaseSensitiveCompare(casesensitive);
    std::sort(begin(), end());
}
