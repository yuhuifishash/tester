#include "memory.h"
#include <iostream>
#include <iomanip> 
#include <sys/sysinfo.h>
#include <unistd.h>
#include <chrono>
#include <cstring> 
#include"get_cmd.h"
#include <cmath>
#include <fstream>

using namespace std;
using namespace chrono;
#define CMD_RESULT_BUF_SIZE 4096


const int kArraySize = 1024 * 1024 * 1024; // 1 GB
const int kNumIterations = 10;

ofstream outputFile;
streambuf* coutBuf;

void redirectCoutToFile() {
    outputFile.open("./result/memory_report.txt",ios::ate);
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file." << endl;
        return;
    }
    coutBuf = cout.rdbuf(); 
    cout.rdbuf(outputFile.rdbuf());
}

void restoreCout() {
    outputFile.close();
    cout.rdbuf(coutBuf);
}


double printMemoryInfo() {
    struct sysinfo memInfo;
    
    if (sysinfo(&memInfo) == 0) {
        return memInfo.totalram;
    } else {
        std::cerr << "get memory information failed.." << std::endl;
    }
    return 0;
}


latency accessLatency() {
    const int N = 100000000;
    int* arr = new int[N];
    latency late;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        arr[i] = 0;
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "Write access latency: "
        << duration << " milliseconds" << endl;
    late.write=duration;
    start = high_resolution_clock::now();
    volatile int val;
    for (int i = 0; i < N; i++) {
        val = arr[i]; 
    }
    val++;
    end = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(end - start).count();
    cout << "Read access latency: "
        << duration << " milliseconds" << endl;

    delete[] arr;

    late.read=duration;
    return late;
}


double memoryBandWidth(){//GB/S

    char *arr = new char[kArraySize];
    auto start = high_resolution_clock::now();
    for (int i = 0; i < kNumIterations; i++) {
        for (int j = 0; j < kArraySize; j++) {
            arr[j]++;
        }
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    double bandwidth = (double)kArraySize * kNumIterations / (double)duration / 1000.0;
    cout << "Memory Bandwidth: " << bandwidth << " GB/s" << endl;
    delete[] arr;
    return bandwidth;
    
}

double Throughput(){

  const int buffer_size = 1024 * 1024 * 1024;  // 1 GB buffer
  char* buffer = new char[buffer_size];
  memset(buffer, 0, buffer_size);
  auto start = std::chrono::high_resolution_clock::now();
  memcpy(buffer, buffer + buffer_size / 2, buffer_size / 2);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  double throughput_gb_s = static_cast<double>(buffer_size) / (static_cast<double>(duration_ns) / 1e9) / 1e6/1024;
  std::cout << "Throughput: " << throughput_gb_s << " GB/s" << std::endl;
  delete[] buffer;
  
  return throughput_gb_s;

}

int sysMemInformation(){

    redirectCoutToFile();
    std::cout <<"Memory Test Result:" <<std::endl;
    
    char result[CMD_RESULT_BUF_SIZE]={0};
    ExecuteCMD("free", result);
    std::cout << result<<endl;
    ExecuteCMD("cat /proc/meminfo", result);
    std::cout << result<<endl;
    
    double memoryscore=0;
    
    double totalMemory=printMemoryInfo()/(1024*1024);
    double baseScore=max(min(40.0,2*log2(totalMemory)+30.0),25.0);
    std::cout << "base Score: "<<baseScore<<"/40.0"<<std::endl;
    latency late=accessLatency();
    double latescore=max(min(10.0,(-3.122*log(late.write)+27.746)),5.0)+max(min(10.0,(-2.024*log(late.read)+18.74)),5.0);
    std::cout << "accessLatency Score: "<< std::setprecision(3)<<latescore<<"/20.0"<<std::endl;

    double bandscore=max(min(20.0,2*(2.381*log( memoryBandWidth() )-4.49)),5.0);
    std::cout << "bandWidth Score: "<< std::setprecision(3)<<bandscore<<"/20.0"<<std::endl;

    double throughputscore=max(min(20.0,2.8854*log( Throughput() )+9.356),10.0);
    std::cout << "Throughput Score: "<< std::setprecision(3)<<throughputscore<<"/20.0"<<std::endl;
    memoryscore=baseScore+latescore+bandscore+throughputscore;
    
    std::cout << "Memory Score: "<< std::setprecision(3)<<memoryscore<<"/100.0"<<std::endl;
    
    int rank=int(memoryscore/10);
    if(rank>9) rank=9;
    switch(rank){
      case 9:
        std::cout << "Rank of Memory: S"<<std::endl;
        break;
      case 8:
        std::cout << "Rank of Memory: A"<<std::endl;
        break;
      case 7:
        std::cout << "Rank of Memory: B"<<std::endl;
        break;
      case 6:
        std::cout << "Rank of Memory: C"<<std::endl;
        break;
      case 5:
        std::cout << "Rank of Memory: D"<<std::endl;
        break;
      default:
        std::cout << "Rank of Memory: E"<<std::endl;
        break;
    }

	restoreCout();
    return 0;

}