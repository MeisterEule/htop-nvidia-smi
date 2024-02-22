#ifndef HEADER_GPUUtilMeter
#define HEADER_GPUUtilMeter
/*
htop - DummyMeter.h
(C) 2004-2011 Hisham H. Muhammad
Released under the GNU GPLv2+, see the COPYING file
in the source distribution for its full text.
*/

#include "Meter.h"

typedef enum {
   GPUUTIL_METER_ACTIVE = 0,
   GPUUTIL_METER_ITEMCOUNT = 1,
} GPUUtilMeterValues;

extern const MeterClass GPUUtilMeter_class;

#endif
