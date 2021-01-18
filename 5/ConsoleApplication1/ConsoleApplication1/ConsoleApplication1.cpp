#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

void MyPing(const char* IP, unsigned int Timeout, unsigned int RequestCount) {
    HANDLE hIP = IcmpCreateFile();
    if (hIP == INVALID_HANDLE_VALUE) {
        WSACleanup();
        return;
    }

    char SendData[32] = "Data for ping";
    int LostPacketsCount = 0;
    unsigned int MaxMS = 0;
    int MinMS = -1;

    PICMP_ECHO_REPLY pIpe = (PICMP_ECHO_REPLY)GlobalAlloc(GHND, sizeof(ICMP_ECHO_REPLY) + sizeof(SendData));
    if (pIpe == 0) {
        IcmpCloseHandle(hIP);
        WSACleanup();
        return;
    }
    pIpe->Data = SendData;
    pIpe->DataSize = sizeof(SendData);
    unsigned long ipaddr = inet_addr(IP);

    for (unsigned int i = 0; i < RequestCount; i++) {
        int dwStatus = IcmpSendEcho(hIP, ipaddr, SendData, sizeof(SendData), NULL, pIpe, sizeof(ICMP_ECHO_REPLY) + sizeof(SendData), Timeout);

        if (dwStatus > 0) {
            for (int i = 0; i < dwStatus; i++) {
                unsigned char* pIpPtr = (unsigned char*)&pIpe->Address;
                cout << "Ответ от " << (int)*(pIpPtr) << "." << (int)*(pIpPtr + 1)
                    << "." << (int)*(pIpPtr + 2) << "." << (int)*(pIpPtr + 3)
                    << ": число байт = " << pIpe->DataSize << " время = " << pIpe->RoundTripTime
                    << "мс TTL = " << (int)pIpe->Options.Ttl;

                MaxMS = (MaxMS > pIpe->RoundTripTime) ? MaxMS : pIpe->RoundTripTime;
                MinMS = (MinMS < (int)pIpe->RoundTripTime&& MinMS >= 0) ? MinMS : pIpe->RoundTripTime;
                cout << endl;
            }
        }
        else {
            if (pIpe->Status) {
                LostPacketsCount++;
                switch (pIpe->Status) {
                case IP_DEST_PORT_UNREACHABLE:
                    cout << "Remote host may be down." << endl;
                    break;
                case IP_REQ_TIMED_OUT:
                    cout << "Request timed out." << endl;
                    break;
                case IP_TTL_EXPIRED_TRANSIT:
                    cout << "TTL expired in transit." << endl;
                    break;
                default:
                    cout << "Error code: %ld" << pIpe->Status << endl;
                    break;
                }
            }
        }
    }

    IcmpCloseHandle(hIP);
    WSACleanup();

    if (MinMS < 0) MinMS = 0;
    cout << endl << "Статистика Ping " << IP;
    cout << "\tПакетов: отправлено = " << RequestCount << ", получено = " << RequestCount - LostPacketsCount
        << ", потеряно = " << LostPacketsCount << "<" << (int)(100 / (float)RequestCount) * LostPacketsCount << " % потерь>, " << endl;
    cout << "Приблизительное время приема-передачи:" << endl << "Минимальное = " << MinMS << "мс, Максимальное = " << MaxMS
        << "мс, Среднее = " << (MaxMS + MinMS) / 2 << "мс" << endl;
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "RUS");
    /*Ping("216.58.206.14", 1000, 10);*/
    MyPing("127.0.0.1", 1000, 10);

    system("pause");
    return 0;
}