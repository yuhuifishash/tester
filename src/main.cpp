#include<iostream>
#include"CPU.h"
#include"memory.h"
#include"disk.h"
#include"get_cmd.h"
#include"arithmetic.h"
using namespace std;

#define CMD_RESULT_BUF_SIZE 4096

int main()
{
    // char result[CMD_RESULT_BUF_SIZE]={0};
    // ExecuteCMD("lscpu", result);
    // printf("%s", result);

    cout<<"start testing, please wait a minute\n";

    cout<<"start disk testing\n";
    test_disk();
    cout<<"finish disk testing\n";

    cout<<"start memory testing\n";
    sysMemInformation();
    cout<<"finish memory testing\n";

    cout<<"start CPU testing\n";
    InstrTestMain();
    cout<<"finish CPU testing\n";

    return 0;
}