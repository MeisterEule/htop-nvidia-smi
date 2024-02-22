#ifndef NVML_API_H
#define NVML_API_H

typedef struct nvmlDevice_st nvmlDevice_t;

typedef struct {
  unsigned long long total;
  unsigned long long free;
  unsigned long long used;
} nvml_memory_t;

typedef struct {
   unsigned int gpu;
   unsigned int memory;
} nvml_util_t;

extern void (*nvmlInit)();
extern void (*getNVMLDeviceCount)();
extern void (*getNVMLDeviceHandleByIndex) (const unsigned int index, nvmlDevice_t **handle);
extern void (*getNVMLMemoryInfo) (nvmlDevice_t *device_handle, nvml_memory_t *memory);
extern void (*getNVMLKernelUtilization) (nvmlDevice_t *device_handle, nvml_util_t *util);

extern nvmlDevice_t *nvml_device_handle;

extern int NVML_INITIALIZED;

extern void Nvml_Init();
extern void Nvml_getMemoryInfo (double *kb_used, double *kb_free, double *kb_total);
extern void Nvml_getKernelUtilization (double *percent);
#endif
