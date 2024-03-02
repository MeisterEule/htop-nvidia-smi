#ifndef GPU_PIDS_H
#define GPU_PIDS_H

#include <stdbool.h>

extern int n_gpu_procs;
extern int *gpu_procs;

void gather_gpu_pids ();
void free_gpu_pids ();
bool is_gpu_pid();

#endif
