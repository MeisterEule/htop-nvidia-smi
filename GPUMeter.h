#ifndef HEADER_GPUMeter
#define HEADER_GPUMeter
/*
htop - DummyMeter.h
(C) 2004-2011 Hisham H. Muhammad
Released under the GNU GPLv2+, see the COPYING file
in the source distribution for its full text.
*/

#include "Meter.h"

typedef enum {
   GPU_METER_ACTIVE = 0,
   GPU_METER_ITEMCOUNT = 1,
} GPUMeterValues;

extern const MeterClass GPUMeter_class;
extern const MeterClass LeftGPUs4Meter_class;

#endif
