#ifndef HEADER_DummyMeter
#define HEADER_DummyMeter
/*
htop - DummyMeter.h
(C) 2004-2011 Hisham H. Muhammad
Released under the GNU GPLv2+, see the COPYING file
in the source distribution for its full text.
*/

#include "Meter.h"

typedef enum {
   GPUMEM_METER_USED = 0,
   GPUMEM_METER_FREE = 1,
   GPUMEM_METER_ITEMCOUNT = 2,
} GPUMemoryMeterValues;

extern const MeterClass GPUMemoryMeter_class;
extern const MeterClass GPUMemoryTextMeter_class;
extern const MeterClass AllGPUsMemory4Meter_class;

#endif
