﻿#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <WS2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define nBuf 536

#define nMeas 66
#define nHT 4

int main(int argc, char* argv[])
{
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %ld\n", iResult);
        return 1;
    }

    //----------------------
    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
        return 1;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    // addrServer.sin_addr.s_addr = inet_addr( "192.168.0.7" );
    inet_pton(AF_INET, "192.168.0.7", &addrServer.sin_addr.s_addr);   //require #include <WS2tcpip.h>
    addrServer.sin_port = htons(4000);

	//----------------------
    // Connect to server.
    iResult = connect( ConnectSocket, (SOCKADDR*) &addrServer, sizeof(addrServer) );
    if ( iResult == SOCKET_ERROR) {
        closesocket (ConnectSocket);
        printf("Unable to connect to server: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    char buf[nBuf] = {};
    char fhData[nHT] = {'D','C','B','A'};
    char feData[nHT] = {'Z','Y','X','W'}; 
    int count;
    
    
    char* head = fhData;
    char* tail = feData;
    char cFrame[nBuf] = {};
    char* cDest = cFrame;

    BOOL findhead = 0;
    UINT32 dFrame[2*nMeas] = {};
    int nohead = 0;
    int head0 = 0;
    int index;
    int imeas = 0;
    int recvCount = 0;
    int currentPosition = 0;


    printf("transfer begin:\n ");
    while (imeas < 100) {
        findhead = 0;
        count = nBuf;
        currentPosition = 0;
        while (count > 0 && (recvCount = recv(ConnectSocket, buf + currentPosition, count, 0)) != SOCKET_ERROR)
        {
            count -= recvCount;
            currentPosition += recvCount;
        }

        //寻找帧头（"ABCD"）
        index = 0;
        while (index <  nBuf)
        {
            if (memcmp(&buf[index], &fhData, nHT) == 0)
            {
                findhead = 1;
                break;
            } 
            else
            {
                index++;
            }
        }
        //如找到帧头，判断在何处找到帧头。
        //如果在Index==0时即找到，则此帧完整，转储；
        //否则，根据当前帧头的位置再读入数据，以收取完整帧数据为目的；此时，再次判断index > nHT，如满足则说明除帧尾之外，还有有效数据需要读取并转储。
        //以此保证数据帧的完整。目前，基本上都是index==0的情况，由最后检查nohead和head0可知。
        if (findhead) {
            if (index == 0)
            {
                memcpy(&cFrame[0], &buf[index+nHT], nBuf - 2*nHT);
                head0++;
            }
            else
            {   
                memcpy(&cFrame[0], &buf[index+nHT], nBuf - index-nHT);
                // receive the remaining frame data
                count = index;
                currentPosition = 0;
                while (count > 0 && (recvCount = recv(ConnectSocket, buf + currentPosition, count, 0)) != SOCKET_ERROR)
                {
                    count -= recvCount;
                    currentPosition += recvCount;
                }
                //count = recv(ConnectSocket, buf, index,0);
                if (index > nHT)
                {
                    memcpy(&cFrame[nBuf - index - nHT], &buf[0], index - nHT);
                }
            }
        }
        else
        {
            nohead++;
        }
        printf(".");

        memcpy(&dFrame[0], &cFrame[0], nMeas*4*2);
        if (imeas % 10 == 0) {
            printf("imeas =  %d\n", imeas);
        }
        if (imeas == 99) {
            printf("-----------------------index = %d \n", index);
            for (UINT i = 0; i < nMeas*2; i++)
            {
                printf("dFrame[%d]=\t", i);
                printf("%X\n", dFrame[i]);
            }
            // 程序测试，用于检查原始数据是否正确
            //for (UINT i = 0; i < nBuf; i++)     // for ect
            //{
            //    printf("cFrame[%d]=", i);
            //    printf("%X\t", cFrame[i]);
            //    printf("buf[%d]=", i);
            //    printf("%X\t\n", buf[i]);
            //}
        }
        imeas++;

    }

    

    printf("\nNumber of no head frame = %d \n", nohead);
    printf("Number of head0         = %d \n", head0);



	//结束连接
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

