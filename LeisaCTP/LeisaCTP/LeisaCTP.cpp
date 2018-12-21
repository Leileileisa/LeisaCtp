// LeisaCTP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "../CTP_API/ThostFtdcUserApiDataType.h"
#include "../CTP_API/ThostFtdcUserApiStruct.h"
#include "../CTP_API/ThostFtdcMdApi.h"
#include "../CTP_API/ThostFtdcTraderApi.h"
#include "../Header/LeisaMdSpi.h"
#include "../Header/LeisaMdApi.h"
#include "../Header/LeisaTraderApi.h"
#include "../Header/LeisaTraderSpi.h"
#include <mutex>
using namespace std;
#pragma comment(lib,"../CTP_API/thostmduserapi.lib")
#pragma comment(lib,"../CTP_API/thosttraderapi.lib")
TThostFtdcInvestorIDType investorID="133522";
TThostFtdcPasswordType password="leisaisapig320";
TThostFtdcBrokerIDType brokerID="9999";
TThostFtdcPasswordType newPassword = "leisaisapig320";
CThostFtdcMdApi* mdApi = nullptr;
CThostFtdcTraderApi* traderApi = nullptr;
char MdFrontAddr[] = "tcp://180.168.146.187:10010";
char TradeFrontAddr[] = "tcp://180.168.146.187:10001";
const char* instrumentID[]= { "sp1906", "zn1905", "cs1905", "CF1905" };
char* mdInstrumentID[4]; // 行情合约代码列表
int instrumentNum = 4;
int requestID = 0;
int main()
{
    std::cout << "Hello World!\n"; 
	for (int i = 0; i < instrumentNum; i++)
	{
		mdInstrumentID[i] = new char[100];
		strcpy_s(mdInstrumentID[i],100,instrumentID[i]);
	}
	traderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	CThostFtdcTraderSpi* traderSpi = new LeisaTraderSpi;
	traderApi->RegisterSpi(traderSpi);
	traderApi->RegisterFront(TradeFrontAddr);
	traderApi->SubscribePublicTopic(THOST_TERT_RESTART);    // 订阅公共流
	traderApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // 订阅私有流
	traderApi->Init();
	
	/*getchar();*/
	/*mdApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CThostFtdcMdSpi *mdSpi = new LeisaMdSpi;
	mdApi->RegisterSpi(mdSpi);
	mdApi->RegisterFront(MdFrontAddr);
	mdApi->Init();*/

	traderApi->Join();
	delete traderSpi;
	traderApi->Release();

	//mdApi->Join();
	//delete mdSpi;
	//mdApi->Release();
	getchar();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
