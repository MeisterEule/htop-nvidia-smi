#include "config.h" // IWYU pragma: keep

#include "GPUMeter.h"

#include "CRT.h"
#include "Object.h"
#include "Platform.h"
#include "XUtils.h"

typedef struct GPUMeterData_ {
   unsigned int ngpus;
   Meter** meters;
} GPUMeterData;


static const int GPUMeter_attributes[] = {
   MEMORY_USED
};

static void AllGPUsMeter_getRange(const Meter* this, int* start, int* count) {
   const GPUMeterData* data = this->meterData;
   unsigned int ngpus = data->ngpus;
   switch (Meter_name(this)[0]) {
      default:
      case 'A': // All
         *start = 0;
         *count = ngpus;
         break;
      case 'L': // First Half
         *start = 0;
         *count = (ngpus + 1) / 2;
         break;
      case 'R': // Second Half
         *start = (ngpus + 1) / 2;
         *count = ngpus / 2;
         break;
   }
}



static void GPUMeterCommonInit (Meter *this, int ncol) {
   GPUMeterData *data = this->meterData;
   if (!data) {
      data = this->meterData = xMalloc(sizeof(GPUMeterData));
      Machine *host = this->host;
      data->ngpus = host->activeGPUs;
      data->meters = xCalloc(data->ngpus, sizeof(Meter*));
   }

   Meter **meters = data->meters;
   int start, count;
   AllGPUsMeter_getRange(this, &start, &count);
   for (int i = 0; i < data->ngpus; i++) {
      if (!meters[i]) {
         meters[i] = Meter_new(this->host, i + 1, (const MeterClass*)Class(GPUMeter));
      }
      Meter_init(meters[i]);
   }

   if (this->mode == 0)
      this->mode = BAR_METERMODE;

   int h = Meter_modes[this->mode]->h;
   this->h = h * ((count + ncol - 1) / ncol);
}

static void GPUMeter_updateValues(Meter* this) {
   memset(this->values, 0, sizeof(double) * GPU_METER_ITEMCOUNT);

   double percent = Platform_setGPUUtilizationValues (this);
   char *buffer = this->txtBuffer;
   size_t size = sizeof(this->txtBuffer);
   int written;

   char gpuUsageBuffer[8] = { 0 };

   xSnprintf(this->txtBuffer, sizeof(this->txtBuffer), "%.1f%%", percent);
}


static void GPUMeter_init (Meter *this) {
   char caption[10];
   xSnprintf(caption, sizeof(caption), "GPU%d", 0);
   Meter_setCaption (this, caption);
}

static void GPUMeterCommonDraw(Meter* this, int x, int y, int w, int ncol) {
   GPUMeterData* data = this->meterData;
   Meter** meters = data->meters;
   int start, count;
   AllGPUsMeter_getRange(this, &start, &count);
   int colwidth = (w - ncol) / ncol + 1;
   int diff = (w - (colwidth * ncol));
   int nrows = (count + ncol - 1) / ncol;
   for (int i = 0; i < count; i++) {
      int d = (i / nrows) > diff ? diff : (i / nrows); // dynamic spacer
      int xpos = x + ((i / nrows) * colwidth) + d;
      int ypos = y + ((i % nrows) * meters[0]->h);
      meters[i]->draw(meters[i], xpos, ypos, colwidth);
   }
}

static void GPUMeterCommonUpdateMode(Meter* this, int mode, int ncol) {
   GPUMeterData* data = this->meterData;
   Meter** meters = data->meters;
   this->mode = mode;
   int h = Meter_modes[mode]->h;
   int start, count;
   AllGPUsMeter_getRange(this, &start, &count);
   for (int i = 0; i < count; i++) {
      Meter_setMode(meters[i], mode);
   }
   this->h = h * ((count + ncol - 1) / ncol);
}

static void QuadColGPUsMeter_draw(Meter* this, int x, int y, int w) {
   GPUMeterCommonDraw(this, x, y, w, 4);
}

static void QuadColGPUsMeter_init(Meter* this) {
   GPUMeterCommonInit(this, 4);
}

static void QuadColGPUsMeter_updateMode(Meter* this, int mode) {
   GPUMeterCommonUpdateMode(this, mode, 4);
}



static void AllGPUsMeter_done(Meter* this) {
   GPUMeterData* data = this->meterData;
   Meter** meters = data->meters;
   int start, count;
   AllGPUsMeter_getRange(this, &start, &count);
   for (int i = 0; i < count; i++)
      Meter_delete((Object*)meters[i]);
   free(data->meters);
   free(data);
}

static void AllGPUsMeter_updateValues(Meter* this) {
   GPUMeterData* data = this->meterData;
   Meter** meters = data->meters;
   int start, count;
   AllGPUsMeter_getRange(this, &start, &count);
   for (int i = 0; i < count; i++)
      Meter_updateValues(meters[i]);
}

static void GPUMeter_getUiName(const Meter* this, char* buffer, size_t length) {
   if (this->param > 0)
      xSnprintf(buffer, length, "%s %u", Meter_uiName(this), this->param);
   else
      xSnprintf(buffer, length, "%s", Meter_uiName(this));
}

const MeterClass GPUMeter_class = {
   .super = {
       .extends = Class(Meter),
       .delete = Meter_delete,
   },
   .updateValues = GPUMeter_updateValues,
   .getUiName = GPUMeter_getUiName,
   .defaultMode = BAR_METERMODE,
   .maxItems = GPU_METER_ITEMCOUNT,
   .total = 100.0,
   .attributes = GPUMeter_attributes,
   .name = "GPU",
   .uiName = "GPU",
   .caption = "GPU",
   .init = GPUMeter_init

};

const MeterClass LeftGPUs4Meter_class = {
   .super = {
      .extends = Class(Meter),
      .delete = Meter_delete,
   },
   .updateValues = AllGPUsMeter_updateValues,
   .defaultMode = CUSTOM_METERMODE,
   .isMultiColumn = true,
   .total = 100.0,
   .attributes = GPUMeter_attributes,
   .name = "LeftGPUs4",
   .uiName = "GPUs (1-4)",
   .caption = "GPU Util",
   .draw = QuadColGPUsMeter_draw,
   .init = QuadColGPUsMeter_init,
   .updateMode = QuadColGPUsMeter_updateMode,
   .done = AllGPUsMeter_done
};

