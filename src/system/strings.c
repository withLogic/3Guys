/*
Copyright (C) 2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "strings.h"

void initStrings(void)
{
	app.strings[ST_CLICK_TO_CONTINUE] = _("Click to continue");
	app.strings[ST_DIAMOND_DESC] = _("A diamond. It's very shiny. I'll have it for lunch.");
	app.strings[ST_STAR_DESC] = _("A star. It looks tasty. Will make a nice side for the diamond.");
	app.strings[ST_LEVEL_NUM] = _("Level %03d / 100");
	app.strings[ST_NUM_MOVES] = _("Moves: %d");
}