#include <dlfcn.h>

#include "NvmlAPI.h"

void (*nvmlInit)();
void (*getNVMLDeviceCount)(unsigned int *ngpus);
void (*getNVMLDeviceHandleByIndex) (const unsigned int index, nvmlDevice_t **handle);
void (*getNVMLMemoryInfo) (nvmlDevice_t *device_handle, nvml_memory_t *memory);
void (*getNVMLKernelUtilization) (nvmlDevice_t *device_handle, nvml_util_t *util);

nvmlDevice_t *nvml_device_handle;

int NVML_INITIALIZED = 0;

void Nvml_Init () {
   int retval;

   void *nvml_solib = dlopen ("/usr/lib/x86_64-linux-gnu/libnvidia-ml.so", RTLD_LAZY);
   *(void**)(&nvmlInit) = dlsym(nvml_solib, "nvmlInit");

   (void)nvmlInit();

   *(void**)(&getNVMLDeviceCount) = dlsym(nvml_solib, "nvmlDeviceGetCount_v2");

   *(void**)(&getNVMLDeviceHandleByIndex) = dlsym(nvml_solib, "nvmlDeviceGetHandleByIndex");
   (void)getNVMLDeviceHandleByIndex(0, &nvml_device_handle);
   
   *(void**)(&getNVMLMemoryInfo) = dlsym(nvml_solib, "nvmlDeviceGetMemoryInfo");
   *(void**)(&getNVMLKernelUtilization) = dlsym(nvml_solib, "nvmlDeviceGetUtilizationRates");

   NVML_INITIALIZED = 1;
}

int Nvml_getDeviceCount () {
   int ngpus;
   getNVMLDeviceCount(&ngpus);
   return ngpus;
}

void Nvml_getMemoryInfo (double *kb_used, double *kb_free, double *kb_total) {
   nvml_memory_t memory_info;  
   getNVMLMemoryInfo (nvml_device_handle, &memory_info);
   *kb_used = (double)memory_info.used / 1024; 
   *kb_free = (double)memory_info.free / 1024; 
   *kb_total = (double)memory_info.total / 1024; 
}

void Nvml_getKernelUtilization (double *percent) {
   nvml_util_t util;
   getNVMLKernelUtilization (nvml_device_handle, &util);
   *percent = util.gpu;
}
