/* htop - DummyMeter.c
(C) 2004-2011 Hisham H. Muhammad
Released under the GNU GPLv2+, see the COPYING file
in the source distribution for its full text.
*/

#include "config.h" // IWYU pragma: keep

#include "GPUMemoryMeter.h"

#include "CRT.h"
#include "Object.h"
#include "Platform.h"
#include "XUtils.h"

enum nvmlReturn_t {
  NVML_SUCCESS                       = 0,
  NVML_ERROR_UNINITIALIZED           = 1,
  NVML_ERROR_INVALID_ARGUMENT        = 2,
  NVML_ERROR_NOT_SUPPORTED           = 3,
  NVML_ERROR_NO_PERMISSION           = 4,
  NVML_ERROR_ALREADY_INITIALIZED     = 5,
  NVML_ERROR_NOT_FOUND               = 6,
  NVML_ERROR_INSUFFICIENT_SIZE       = 7,
  NVML_ERROR_INSUFFICIENT_POWER      = 8,
  NVML_ERROR_DRIVER_NOT_LOADED       = 9,
  NVML_ERROR_TIMEOUT                 = 10,
  NVML_ERROR_IRQ_ISSUE               = 11,
  NVML_ERROR_LIBRARY_NOT_FOUND       = 12,
  NVML_ERROR_FUNCTION_NOT_FOUND      = 13,
  NVML_ERROR_CORRUPTED_INFOROM       = 14,
  NVML_ERROR_GPU_IS_LOST             = 15,
  NVML_ERROR_RESET_REQUIRED          = 16,
  NVML_ERROR_OPERATING_SYSTEM        = 17,
  NVML_ERROR_LIB_RM_VERSION_MISMATCH = 18,
  NVML_ERROR_IN_USE                  = 19,
  NVML_ERROR_MEMORY                  = 20,
  NVML_ERROR_NO_DATA                 = 21,
  NVML_ERROR_VGPU_ECC_NOT_SUPPORTED  = 22,
  NVML_ERROR_UNKNOWN                 = 999,
};

static const int GPUMemoryMeter_attributes[] = {
   MEMORY_USED
};

static void GPUMemoryMeter_updateValues(Meter* this) {
   Platform_setGPUMemoryValues (this);
   char *buffer = this->txtBuffer;
   size_t size = sizeof(this->txtBuffer);
   int written;

   written = Meter_humanUnit(buffer, this->values[GPUMEM_METER_USED], size);
   METER_BUFFER_CHECK(buffer, size, written);
   METER_BUFFER_APPEND_CHR(buffer, size, '/');
   Meter_humanUnit(buffer, this->total, size);
}

static void GPUMemoryTextMeter_updateValues (Meter *this) {
   double kb_used, kb_free, kb_total;

   Nvml_getMemoryInfo (&kb_used, &kb_free, &kb_total);
   xSnprintf (this->txtBuffer, sizeof(this->txtBuffer), "used: %lf, free: %lf, total: %lf", kb_used, kb_free, kb_total);
}

static void GPUMemoryMeter_display (const Object *cast, RichString *out) {
}

const MeterClass GPUMemoryMeter_class = {
   .super = {
      .extends = Class(Meter),
      .delete = Meter_delete,
   },
   .updateValues = GPUMemoryMeter_updateValues,
   .defaultMode = BAR_METERMODE,
   .maxItems = GPUMEM_METER_ITEMCOUNT,
   .total = 100.0,
   .attributes = GPUMemoryMeter_attributes,
   .init = Platform_initGPU,
   .name = "GPUMem",
   .uiName = "GPUMem",
   .caption = "GPU Mem"
};

const MeterClass GPUMemoryTextMeter_class = {
   .super = {
      .extends = Class(Meter),
      .delete = Meter_delete,
   },
   .updateValues = GPUMemoryTextMeter_updateValues,
   .defaultMode = TEXT_METERMODE,
   .maxItems = GPUMEM_METER_ITEMCOUNT,
   .total = 100.0,
   .attributes = GPUMemoryMeter_attributes,
   .init = Platform_initGPU,
   .name = "GPUMemText",
   .uiName = "GPUMemText",
   .caption = "GPU Mem: "
};
