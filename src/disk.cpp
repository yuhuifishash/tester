#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <cstdio>
#include "disk.h"



void testDiskPerformance(const std::string& filePath, std::vector<double>& timings, double& iops, double& queueDepth) {
    const int bufferSize = 1024 * 1024; 
    const int iterations = 10; 

    std::ofstream outputFile(filePath, std::ios::out | std::ios::binary);
    if (!outputFile) {
        std::cerr << "cant open file to write!" << std::endl;
        return;
    }


    std::vector<char> buffer(bufferSize, 'A');
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        outputFile.write(buffer.data(), bufferSize);
    }
    auto end = std::chrono::steady_clock::now();
    double writeTime = std::chrono::duration<double>(end - start).count() / iterations;
    timings.push_back(writeTime);

    outputFile.close();


    std::ifstream inputFile(filePath, std::ios::in | std::ios::binary);
    if (!inputFile) {
        std::cerr << "cant open file to read!" << std::endl;
        return;
    }

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        inputFile.read(buffer.data(), bufferSize);
    }
    end = std::chrono::steady_clock::now();
    double readTime = std::chrono::duration<double>(end - start).count() / iterations;
    timings.push_back(readTime);

    inputFile.close();


    double totalTime = writeTime + readTime;
    double totalOperations = bufferSize * iterations;
    iops = totalOperations / totalTime;


    queueDepth = iops * (totalTime / iterations);
}


std::string getPerformanceScore(double time, double& percentile) {
    if (time < 0.001) {
        percentile = 95.0;
        return "S";
    }
    else if (time < 0.002) {
        percentile = 85.0;
        return "A";
    }
    else if (time < 0.01) {
        percentile = 70.0;
        return "B";
    }
    else if (time < 0.05) {
        percentile = 50.0;
        return "C";
    }
    else {
        percentile = 30.0;
        return "D";
    }
}

int test_disk() {
    std::ofstream outputFile("result/disk_report.txt"); 

    std::string command = "lsblk"; 
    char buffer[128]; 


    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "fail to execute" << std::endl;
        return 1;
    }

    outputFile << "The information of your hard drive device is:" << std::endl;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        outputFile << buffer;
    }
    outputFile << std::endl;


    pclose(pipe);

    std::string filePath = "disk_performance_test.bin"; 
    std::vector<double> timings; 
    double iops = 0.0; // IOPS
    double queueDepth = 0.0; 


    testDiskPerformance(filePath, timings, iops, queueDepth);


    double writeTime = timings[0];
    double readTime = timings[1];
    double alltime = writeTime + readTime;
    double writePercentile = 0.0;
    double readPercentile = 0.0;
    std::string writeScore = getPerformanceScore(writeTime, writePercentile);
    std::string readScore = getPerformanceScore(readTime, readPercentile);
    std::string score = getPerformanceScore(alltime, readPercentile);


    outputFile << "Disk performance evaluation results:" << std::endl;
    outputFile << "Write time: " << writeTime << " In seconds, score: " << writeScore << std::endl;
    outputFile << "Reading time: " << readTime << " In seconds, score: " << readScore << std::endl;
    outputFile << "IOPS: " << iops << std::endl;
    outputFile << "Queue depth:" << queueDepth << std::endl;
    outputFile << "On the scale of S,A,B,C, and D, your total score is: " << score << std::endl;


    std::remove(filePath.c_str());

    outputFile.close();

    return 0;
}