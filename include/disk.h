#ifndef DISK_PERFORMANCE_H
#define DISK_PERFORMANCE_H

#include <string>
#include <vector>

void testDiskPerformance(const std::string& filePath, std::vector<double>& timings, double& iops, double& queueDepth);
std::string getPerformanceScore(double time, double& percentile);
int test_disk();

#endif