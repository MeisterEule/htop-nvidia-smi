#include <stdlib.h>
#include <stdbool.h>

#include "linux/NvmlAPI.h"
#include "linux/gpu_pids.h"

int n_gpu_procs;
int *gpu_procs = NULL;

void gather_gpu_pids () {
   Nvml_GetRunningProcesses (&n_gpu_procs, NULL);
   if (n_gpu_procs > 0) {
      nvml_process_info_t *infos = (nvml_process_info_t *)malloc(n_gpu_procs * sizeof(nvml_process_info_t));
      gpu_procs = (int *)malloc(n_gpu_procs * sizeof(int)); 
      Nvml_GetRunningProcesses (&n_gpu_procs, infos);
      for (int i = 0; i < n_gpu_procs; i++) {
         gpu_procs[i] = infos[i].computeInstanceId;
      }
      free (infos);
   }
}

void free_gpu_pids() {
   n_gpu_procs = 0;
   free(gpu_procs);
   gpu_procs = NULL;
}

bool is_gpu_pid (int pid) {
   for (int i = 0; i < n_gpu_procs; i++) {
      if (pid == gpu_procs[i]) return true;
   }
   return false;
}
