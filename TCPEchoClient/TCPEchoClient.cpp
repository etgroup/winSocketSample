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
    char feData[nHT] = {'W','X','Y','Z'}; 
    int count;
    
    
    char* head = fhData;
    char* tail = feData;
    char cFrame[nBuf] = {};
    char* cDest = cFrame;
    BOOL findhead = 0;
    int nohead = 0;
    int head0 = 0;
    int index;
    int imeas = 0;
    int recvCount = 0;
    int currentPosition = 0;


    printf("transfer begin:\n ");
    while (imeas < 1000) {
        findhead = 0;
        count = nBuf;
        currentPosition = 0;
        while (count > 0 && (recvCount = recv(ConnectSocket, buf + currentPosition, count, 0)) != SOCKET_ERROR)
        {
            count -= recvCount;
            currentPosition += recvCount;
        }

        //count = recv(ConnectSocket, buf, nBuf, 0);
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

        if (findhead) {
            if (index == 0)
            {
                memcpy(&cFrame[0], &buf[index+nHT], nBuf - 2*nHT);
                head0++;
            }
            else
            {   
                memcpy(&cFrame[0], &buf[index+nHT], nBuf - index-2*nHT);
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
                    memcpy(&cFrame[nBuf - index - 2 * nHT], buf, index - nHT);
                }
            }
            //printf("-----------------------index = %d \n", index);
            //for (UINT i = 0; i < nBuf-10*nHT; i++)     // for ect
            //{
            //    printf("cFrame[%d]=", i);
            //    printf("%X\t\n", cFrame[i]);
            //}
            //for (UINT i = nBuf - 3*nHT; i < nBuf-2*nHT; i++)     // for ect
            //{
            //    printf("cFrame[%d]=", i);
            //    printf("%X\t\n", cFrame[i]);
            //}
        }
        else
        {
            nohead++;
        }
        //printf(".");

        //else
        //{
        //    printf("------------------------------------------NO HEAD < 8\n");

        //    for (UINT i = 0; i < nBuf; i++)     // for ect
        //    {
        //        printf("buf[%d]=", i);
        //        printf("%X\t\n", buf[i]);
        //    }
        //    printf("------------------------------------------NO HEAD < 8\n");
        //}
        //printf("-----------------------------------------------------iMeas = %d \n", imeas);
        imeas++;

    }


    printf("\nNumber of no head frame = %d \n", nohead);
    printf("Number of head0         = %d \n", head0);



	//结束连接
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

