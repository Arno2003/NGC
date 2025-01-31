#ifndef CPUUSAGE_H
#define CPUUSAGE_H

#include <stdbool.h>
#include <pthread.h>

// Function declarations
void iniArr();
void setUsage();
void* get_cpu_usage(void* arg);

#endif // CPUUSAGE_H
