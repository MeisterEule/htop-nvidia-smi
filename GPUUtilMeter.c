#include "config.h" // IWYU pragma: keep

#include "GPUUtilMeter.h"

#include "CRT.h"
#include "Object.h"
#include "Platform.h"
#include "XUtils.h"

static const int GPUUtilMeter_attributes[] = {
   MEMORY_USED
};

static void GPUUtilMeter_updateValues(Meter* this) {
   Platform_setGPUUtilizationValues (this);
   char *buffer = this->txtBuffer;
   size_t size = sizeof(this->txtBuffer);
   int written;

   written = Meter_humanUnit(buffer, this->values[GPUUTIL_METER_ACTIVE], size);
   METER_BUFFER_CHECK(buffer, size, written);
   METER_BUFFER_APPEND_CHR(buffer, size, '/');
   Meter_humanUnit(buffer, this->total, size);
}

const MeterClass GPUUtilMeter_class = {
   .super = {
      .extends = Class(Meter),
      .delete = Meter_delete,
   },
   .updateValues = GPUUtilMeter_updateValues,
   .defaultMode = BAR_METERMODE,
   .maxItems = GPUUTIL_METER_ITEMCOUNT,
   .total = 100.0,
   .attributes = GPUUtilMeter_attributes,
   .init = Platform_initGPU,
   .name = "GPUUtil",
   .uiName = "GPUUtil",
   .caption = "GPU Util"
};

