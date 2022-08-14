#ifndef WATCHY_CHURCHYEAR_H
#define WATCHY_CHURCHYEAR_H

#include <Watchy.h>
#include "Fonts/DSEG7_Classic_Bold_25.h"
#include "Fonts/DSEG7_Classic_Regular_15.h"
#include "Fonts/DSEG7_Classic_Regular_39.h"
#include "Fonts/icons.h"
#include "Fonts/Seven_Segment10pt7b.h"


class WatchyChurchYear : public Watchy{
	using Watchy::Watchy;
	public:
		void drawWatchFace();
		void drawTime();
		void drawDate();
		void drawSteps();
		void drawWeather();
		void drawBattery();
};

#endif
