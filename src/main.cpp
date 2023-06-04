#include<iostream>
#include"instruction.h"
#include"memory.h"
#include"disk_IO.h"
#include"multi_thread.h"
#include"get_cmd.h"
using namespace std;

#define CMD_RESULT_BUF_SIZE 4096

int main()
{
    char result[CMD_RESULT_BUF_SIZE]={0};

    ExecuteCMD("lscpu", result);
    printf("%s", result);

    return 0;
}