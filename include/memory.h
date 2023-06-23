#ifndef MEMORY_H
#define MEMORY_H

struct latency{
  double read;
  double write;
};

void redirectCoutToFile();
void restoreCout();

double printMemoryInfo();
latency accessLatency();
double memoryBandWidth();
double Throughput();
int sysMemInformation();


#endif