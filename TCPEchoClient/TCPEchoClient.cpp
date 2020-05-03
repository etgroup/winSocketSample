#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <io.h>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define nBuf 536

#define nMeas 66
#define nHT 4
char* find_aim_str(char* head, char* tail, char* src)
{
    char* temp;

    int h_len = strlen(head);
    int t_len = strlen(tail);

    while (*src != '\0')
    {
        if (strncmp(src, head, h_len) == 0)
        {
            temp = src;
            src = src + h_len;
            while (*src != '\0')
            {
                if (strncmp(src, tail, t_len) == 0)
                {
                    *(src + t_len) = '\0';
                    return temp;
                }
                src++;
            }
            return NULL;
        }
        src++;
    }
    return NULL;
}
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
    addrServer.sin_addr.s_addr = inet_addr( "192.168.0.7" );
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
    char fhData[nHT] = {'A','B','C','D'};
    char feData[nHT] = { 'W','X','Y','Z' };
    int count;
    
    
    char* head = fhData;
    char* tail = feData;
    char cFrame[nBuf] = {};
    char* cDest = cFrame;
    BOOL findhead = 0;
    int index;
    int imeas = 0;

    while (imeas < 1000) {
        while (findhead == 0) {
            count = recv(ConnectSocket, buf, nBuf, 0);
            index = 0;
            while (index < nHT)
            {
                if (memcmp(&buf[index], &fhData, nHT) == 0)
                {
                    index += nHT;
                    findhead = 1;
                    break;
                }
                else
                {
                    index++;
                }
            }
        }

        if (findhead) {
            memcpy(&cFrame[0], &buf[index], nBuf - index);

            printf("------------------------------------------TIME 1: index = %d \n", index);
            //for (UINT i = 0; i < nBuf; i++)     // for ect
            //{
            //    printf("buf[%d]=", i);
            //    printf("%X\t\t", buf[i]);
            //    printf("cFrame[%d]=", i);
            //    printf("%X\n", cFrame[i]);
            //}
            printf("------------------------------------------TIME 1: index = %d \n", index);

            if (index > 4)
            {
                count = recv(ConnectSocket, buf, index - 4, 0);
                memcpy(&cFrame[nBuf - index], buf, index - 4);
                printf("------------------------------------------TIME 2: index = %d \n", index);
                for (UINT i = 0; i < nBuf; i++)     // for ect
                {
                    printf("buf[%d]=", i);
                    printf("%X\t\t", buf[i]);
                    printf("cFrame[%d]=", i);
                    printf("%X\n", cFrame[i]);
                }
                printf("------------------------------------------TIME 2: index = %d \n", index);

            }
        }
        else
        {
            for (UINT i = 0; i < nBuf; i++)     // for ect
            {
                printf("buf[%d]=", i);
                printf("%X\t\n", buf[i]);
            }
            printf("Find no head of frame\n");
        }
        imeas++;
        printf("-----------------------------------------------------iMeas = %d \n", imeas);

    }



    if (0)
    {
        INT nCombByte = 0;
        BYTE* addr = (BYTE*)&nCombByte;

        // StartTime
        //ETData->lStartTime = InBuffer[0]+InBuffer[1]*256+InBuffer[2]*65536+InBuffer[3]*16777216;

        // EndTime
        //ETData->lEndTime = InBuffer[nTotalMeas-4]+InBuffer[nTotalMeas-3]*256+InBuffer[nTotalMeas-2]*65536+InBuffer[nTotalMeas-1]*16777216;
        //memcpy(&(ETData->lEndTime), buf[nTotalMeas - 4], 4 * sizeof(BYTE));
        float ET_MEAS[nMeas];

        const UINT s = 4;   // s stand for step
        for (UINT i = 0; i < nBuf; i++)     // for ect
        {   
            printf("buf[%d]=", i);
            printf("%X\t\t", buf[i]);
            printf("cFrame[%d]=", i);
            printf("%X\n", cFrame[i]);
        }


    }
	//结束连接
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

