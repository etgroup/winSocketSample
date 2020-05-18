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
	//以一个无限循环的方式，不停地接收输入，发送到server
	while(1)
	{
		int count = _read (0, buf, 1024);//从标准输入读入
		if(count<=0)break;
		int sendCount,currentPosition=0;
		while( count>0 && (sendCount=send(ConnectSocket ,buf+currentPosition,count,0))!=SOCKET_ERROR)
		{
			count-=sendCount;
			currentPosition+=sendCount;
		}
		if(sendCount==SOCKET_ERROR)break;
		
		count =recv(ConnectSocket ,buf,1024,0);
		if(count==0)break;//被对方关闭
		if(count==SOCKET_ERROR)break;//错误count<0
		buf[count]='\0';
		printf("%s",buf);
	}
	//结束连接
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

    clock_t start, end;   //clock_t 鏄痗lock()鐨勮繑鍥炲彉閲忕被鍨�
    start = clock();      //鎹曟崏寰幆娈靛紑濮嬬殑鏃堕棿

    while (imeas < 100) {
        findhead = 0;
        count = nSendByte;
        currentPosition = 0;
        while (count > 0 && (recvCount = recv(ConnectSocket, buf + currentPosition, count, 0)) != SOCKET_ERROR)
        {
            count -= recvCount;
            currentPosition += recvCount;
        }

        //瀵绘壘甯уご锛�"ABCD"锛�
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
        //濡傛壘鍒板抚澶达紝鍒ゆ柇鍦ㄤ綍澶勬壘鍒板抚澶淬��
        //濡傛灉鍦↖ndex==0鏃跺嵆鎵惧埌锛屽垯姝ゅ抚瀹屾暣锛岃浆鍌紱
        //鍚﹀垯锛屾牴鎹綋鍓嶅抚澶寸殑浣嶇疆鍐嶈鍏ユ暟鎹紝浠ユ敹鍙栧畬鏁村抚鏁版嵁涓虹洰鐨勶紱姝ゆ椂锛屽啀娆″垽鏂璱ndex > nHT锛屽婊¤冻鍒欒鏄庨櫎甯у熬涔嬪锛岃繕鏈夋湁鏁堟暟鎹渶瑕佽鍙栧苟杞偍銆�
        //浠ユ淇濊瘉鏁版嵁甯х殑瀹屾暣銆傜洰鍓嶏紝鍩烘湰涓婇兘鏄痠ndex==0鐨勬儏鍐碉紝鐢辨渶鍚庢鏌ohead鍜宧ead0鍙煡銆�

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
        //         绋嬪簭娴嬭瘯锛岀敤浜庢鏌ュ師濮嬫暟鎹槸鍚︽纭�
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

	// 浠ヤ笂锛屾槸鍦ㄥ瓧鑺傛祦鍔ㄧ殑灞傚皢鏈夋晥淇℃伅锛堝抚澶淬�佸抚灏句箣闂寸殑鏈夋晥鏁版嵁瀛楄妭锛夋彁鍙栧嚭鏉ワ紝瀛樺埌cFrame鏁扮粍涓��
	// 浠ヤ笅锛屽垯鏄皢cFrame鏁扮粍杞崲涓篠truct缁撴瀯浣撲腑鐨勬暟鎹甪loat ET.data1鍜孍T.data2锛涙澶凷truct瀵瑰簲涓嬩綅鏈虹殑Struct缁撴瀯浣撱��
	// 杩涗竴姝ワ紝鍙互鑰冭檻灏哻Frame杩欎竴姝ョ渷鎺夈��
	    
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

                    printf("%s", "鈻�");
                    barl--;
                }
            printf("\n");
        }


    }

    end = clock();   //鎹曟崏寰幆娈电粨鏉熺殑鏃堕棿
    printf ( "Elasped time, %d\n", (end - start) / CLK_TCK);  //涓ょ鏃堕棿鐩稿噺鍐嶉櫎浠ュ父閲�

    printf("\nNumber of no head frame = %d \n", nohead);
    printf("Number of head0         = %d \n", head0);




	//缁撴潫杩炴帴
>>>>>>> Stashed changes
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

