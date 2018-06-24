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

#include "player.h"

static void handleEntityClick(Entity *e);
static int addRouteNode(void);
void clearRoute(void);
static void cancelLastNode(void);
static void doControls(void);
static int isWalkableByGuy(void);

static SDL_Point lastRouteNode;

void initPlayer(void)
{
	clearRoute();
}

void doPlayer(void)
{
	if (!level.walkRoute)
	{
		doControls();
	}
}

static void doControls(void)
{
	Entity *e;
	float x, y;
	
	if (app.mouse.button[SDL_BUTTON_LEFT])
	{
		level.message = NULL;
		
		x = app.mouse.x;
		x /= app.scaleX;
		x -= LEVEL_RENDER_X;
		x /= TILE_SIZE;
		
		y = app.mouse.y;
		y /= app.scaleY;
		y -= LEVEL_RENDER_Y;
		y /= TILE_SIZE;
		
		if (level.routeIndex == 0)
		{
			e = getEntityAt(x, y);
			
			if (e != NULL)
			{
				handleEntityClick(e);
			}
		}
		else
		{
			if ((int)x != lastRouteNode.x || (int)y != lastRouteNode.y)
			{
				lastRouteNode.x = x;
				lastRouteNode.y = y;
				
				addRouteNode();
				
				cancelLastNode();
			}
		}
	}
	else if (level.routeIndex > 0)
	{
		level.walkRoute = 1;
		
		level.routeIndex = 0;
	}
}

static void handleEntityClick(Entity *e)
{
	switch (e->type)
	{
		case ET_RED_GUY:
		case ET_YELLOW_GUY:
		case ET_GREEN_GUY:
			if (e == level.guy)
			{
				lastRouteNode.x = e->x;
				lastRouteNode.y = e->y;
				addRouteNode();
			}
			else
			{
				level.guy = e;
			}
			break;
			
		default:
			if (level.routeIndex == 0 && e->describe != NULL)
			{
				e->describe();
			}
			break;
	}
}

static int addRouteNode(void)
{
	int i, n;
	Entity **candidates;
	
	if (lastRouteNode.x < 0 || lastRouteNode.x >= MAP_WIDTH || lastRouteNode.y < 0 || lastRouteNode.y >= MAP_HEIGHT || level.data[lastRouteNode.x][lastRouteNode.y] == TILE_WALL || !isWalkableByGuy())
	{
		return 0;
	}
	
	if (level.routeIndex > 1)
	{
		/* too far away from last node */
		if (fabs(level.route[level.routeIndex - 1].x - lastRouteNode.x) > 1 || fabs(level.route[level.routeIndex - 1].y - lastRouteNode.y) > 1)
		{
			return 0;
		}

		/* don't allow diagonals */
		if (fabs(level.route[level.routeIndex - 1].x - lastRouteNode.x) == 1 && fabs(level.route[level.routeIndex - 1].y - lastRouteNode.y) == 1)
		{
			return 0;
		}
	}
	
	candidates = getEntitiesAt(lastRouteNode.x, lastRouteNode.y, &n);
	
	for (i = 0 ; i < n ; i++)
	{
		if (level.guy != candidates[i] && candidates[i]->isBlocking())
		{
			return 0;
		}
	}
	
	for (i = 0 ; i < MAP_WIDTH * MAP_HEIGHT ; i++)
	{
		if (level.route[i].x == lastRouteNode.x && level.route[i].y == lastRouteNode.y)
		{
			return 0;
		}
		
		if (level.route[i].x == -1)
		{
			level.route[level.routeIndex].x = lastRouteNode.x;
			level.route[level.routeIndex].y = lastRouteNode.y;
			level.routeIndex++;
			return 1;
		}
	}
	
	return 0;
}

static int isWalkableByGuy(void)
{
	switch (level.data[lastRouteNode.x][lastRouteNode.y])
	{
		case TILE_RED:
			return level.guy->type == ET_RED_GUY;
		
		case TILE_YELLOW:
			return level.guy->type == ET_YELLOW_GUY;
		
		case TILE_GREEN:
			return level.guy->type == ET_GREEN_GUY;
			
		default:
			return 1;
	}
}

static void cancelLastNode(void)
{
	if (level.routeIndex > 1)
	{
		if (level.route[level.routeIndex - 2].x == lastRouteNode.x && level.route[level.routeIndex - 2].y == lastRouteNode.y)
		{
			level.route[level.routeIndex - 1].x = -1;
			level.route[level.routeIndex - 1].y = -1;
			
			level.routeIndex--;
		}
	}
}

void clearRoute(void)
{
	int i;
	
	for (i = 0 ; i < MAP_WIDTH * MAP_HEIGHT ; i++)
	{
		level.route[i].x = level.route[i].y = -1;
	}
	
	level.routeIndex = 0;
}