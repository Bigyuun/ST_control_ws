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
    // kk[0] = htonl(ca[0]);

    char send_msg_[512] = {0,};
    static char send_msg_2[512];

    for(int i=0; i<30; i++)
    {
        printf(" %d", send_msg_[i]);
    }

    for(int i=0; i<30; i++)
    {
        printf(" %d", send_msg_2[i]);
    }

    std::cout << "Hi daeyun" << std::endl;

    return 0;

}