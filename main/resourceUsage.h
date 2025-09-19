#ifndef RESOURCEUSAGE_H
#define RESOURCEUSAGE_H

#include <stdbool.h>
#include <pthread.h>

// Function declarations
void iniArr();
void setUsage();
void* getResourceUsage(void* arg);

#endif // RESOURCEUSAGE_H
