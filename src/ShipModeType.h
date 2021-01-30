#pragma once
#ifndef __SHIP_MODE__
#define __SHIP_MODE__
enum ShipModeType
{
	MODELESS,
	SEEK = 1,
	ARRIVE,
	FLEE,
	NUM_OF_MODES
};
#endif /* defined (__SHIP_MODE__) */
