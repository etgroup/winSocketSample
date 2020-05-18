#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <io.h>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 0), &wsaData);
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
<<<<<<< Updated upstream
    addrServer.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    addrServer.sin_port = htons(20131);
=======
    // addrServer.sin_addr.s_addr = inet_addr( "192.168.0.7" );
    inet_pton(AF_INET, "192.168.1.103", &addrServer.sin_addr.s_addr);   //require #include <WS2tcpip.h>
    addrServer.sin_port = htons(23);
>>>>>>> Stashed changes

	//----------------------
    // Connect to server.
    iResult = connect( ConnectSocket, (SOCKADDR*) &addrServer, sizeof(addrServer) );
    if ( iResult == SOCKET_ERROR) {
        closesocket (ConnectSocket);
        printf("Unable to connect to server: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

<<<<<<< Updated upstream
	char buf[1024+1];
	//��һ������ѭ���ķ�ʽ����ͣ�ؽ������룬���͵�server
	while(1)
	{
		int count = _read (0, buf, 1024);//�ӱ�׼�������
		if(count<=0)break;
		int sendCount,currentPosition=0;
		while( count>0 && (sendCount=send(ConnectSocket ,buf+currentPosition,count,0))!=SOCKET_ERROR)
		{
			count-=sendCount;
			currentPosition+=sendCount;
		}
		if(sendCount==SOCKET_ERROR)break;
		
		count =recv(ConnectSocket ,buf,1024,0);
		if(count==0)break;//���Է��ر�
		if(count==SOCKET_ERROR)break;//����count<0
		buf[count]='\0';
		printf("%s",buf);
	}
	//��������
=======
    char buf[nSendByte] = {};
    char fhData[nHT] = {'D','C','B','A'};
    char feData[nHT] = {'Z','Y','X','W'}; 
    int count;
    
    
    char* head = fhData;
    char* tail = feData;
    char cFrame[nSendByte] = {};
    char* cDest = cFrame;

    BOOL findhead = 0;
    UINT32 dFrame[2*nMeas] = {};
    int nohead = 0;
    int head0 = 0;
    int index;
    int imeas = 0;
    int recvCount = 0;
    int currentPosition = 0;
    int barl = 0;
    struct ETFrame {
        UINT32 head;
        FLOAT data1[nMeas];
        FLOAT data2[nMeas];
        UINT32 tail;
    } ET, * ptET;

    printf("transfer begin:\n ");

    clock_t start, end;   //clock_t 是clock()的返回变量类型
    start = clock();      //捕捉循环段开始的时间

    while (imeas < 100) {
        findhead = 0;
        count = nSendByte;
        currentPosition = 0;
        while (count > 0 && (recvCount = recv(ConnectSocket, buf + currentPosition, count, 0)) != SOCKET_ERROR)
        {
            count -= recvCount;
            currentPosition += recvCount;
        }

        //寻找帧头（"ABCD"）
        index = 0;
        while (index <  nSendByte)
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
            memcpy(&(ET.head), &fhData, 4);
            memcpy(&(ET.tail), &feData, 4);

            if (index == 0)
            {
                memcpy(&cFrame[0], &buf[index+nHT], nSendByte - 2*nHT);
                head0++;
            }
            else
            {   
                memcpy(&cFrame[0], &buf[index+nHT], nSendByte - index-nHT);
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
                    memcpy(&cFrame[nSendByte - index - nHT], &buf[0], index - nHT);
                }
            }
        }
        else
        {
            nohead++;
        }
        printf(".");

 

        //memcpy(&dFrame[0], &cFrame[0], nMeas*4);
        //if (imeas % 10 == 0) {
        //    printf("imeas =  %d\n", imeas);
        //}
        //if (imeas == 99) {
        //    printf("-----------------------index = %d \n", index);
        //    for (UINT i = 0; i < nMeas*2; i++)
        //    {
        //        printf("dFrame[%d]=\t", i);
        //        printf("%f\n", dFrame[i]);
        //    }
        //         程序测试，用于检查原始数据是否正确
        //    for (UINT i = 0; i < nSendByte; i++)     // for ect
        //    {
        //        printf("cFrame[%d]=", i);
        //        printf("%X\t", cFrame[i]);
        //        printf("buf[%d]=", i);
        //        printf("%2X\t\n", buf[i]);
        //    }
        //    memcpy(&(ET.data1[0]), &cFrame[0], nMeas * 4);
        //    for (UINT i = 0; i < nMeas; i++)
        //    {
        //        printf("ET.data1[%d]=\t", i);
        //        printf("%f\n", ET.data1[i]);
        //    }        
        //}

        

        imeas++;

	// 以上，是在字节流动的层将有效信息（帧头、帧尾之间的有效数据字节）提取出来，存到cFrame数组中。
	// 以下，则是将cFrame数组转换为Struct结构体中的数据float ET.data1和ET.data2；此处Struct对应下位机的Struct结构体。
	// 进一步，可以考虑将cFrame这一步省掉。
	    
        if (bADC1) {
            memcpy(&(ET.data1[0]), &cFrame[0], nMeas * 4);
			memcpy(&(ETData->fData[0]), &cFrame[0], nMeas * 4);
        }

        if (bADC2) {
            memcpy(&(ET.data2[0]), &cFrame[nMeas * 4], nMeas * 4);
			memcpy(&(ETData->fData[nMeas * 4]), &cFrame[nMeas * 4], nMeas * 4);
        }
        Sleep(100);
        system("cls");
        printf("\n\n\n\n\n#\tOTR\t DATA\t \n");
        for (UINT i = 0; i < nMeas; i++)
        {
                printf("%d\t%f\t", i, ET.data1[i]);
                printf("%f\t", ET.data2[i]);
                barl = ET.data2[i]/150;
                while (barl > 0) {

                    printf("%s", "■");
                    barl--;
                }
            printf("\n");
        }


    }

    end = clock();   //捕捉循环段结束的时间
    printf ( "Elasped time, %d\n", (end - start) / CLK_TCK);  //两端时间相减再除以常量

    printf("\nNumber of no head frame = %d \n", nohead);
    printf("Number of head0         = %d \n", head0);




	//结束连接
>>>>>>> Stashed changes
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

