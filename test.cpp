#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>

int main()
{
    char a[256];

    //double b = a & 0xFFFF;

    long ca[512]={0};
    ca[0] = 0x04030201;
    long kk[512]={0};
    kk[0] = htonl(ca[0]);

    return 0;

}