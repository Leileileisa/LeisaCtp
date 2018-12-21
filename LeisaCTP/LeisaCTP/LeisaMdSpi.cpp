#include "pch.h"
#include "../Header/LeisaMdSpi.h"
#include "../CTP_API/ThostFtdcUserApiDataType.h"
#include "../CTP_API/ThostFtdcUserApiStruct.h"
#include "../CTP_API/ThostFtdcMdApi.h"
#include "../CTP_API/ThostFtdcTraderApi.h"
#include <iostream>
#include <fstream>
extern TThostFtdcInvestorIDType investorID;
extern TThostFtdcBrokerIDType brokerID;
extern TThostFtdcPasswordType password;
extern CThostFtdcMdApi *mdApi;
extern char* mdInstrumentID[4];
extern int instrumentNum;
extern int requestID;
void LeisaMdSpi::OnFrontConnected() {
	std::cout << "OnFrontConnected" << std::endl;
	CThostFtdcReqUserLoginField loginField;
	memset(&loginField,0,sizeof(loginField));
	strcpy_s(loginField.BrokerID,brokerID);
	strcpy_s(loginField.UserID, investorID);
	strcpy_s(loginField.Password, password);
	int result=mdApi->ReqUserLogin(&loginField, requestID++);
	if (!result) {
		std::cout << "ConnectSuccess!" << std::endl;
	}
	else {
		std::cout << "ConnectFail!" << std::endl;
	}
}
void LeisaMdSpi::OnFrontDisConnected(int reason) {
	std::cerr << "DisConnected" << std::endl;
	std::cerr << "ErrorCode�� " << reason << std::endl;
}
void LeisaMdSpi::OnHeartBeatWarning(int timeLapse) {
	std::cerr << "HeartBeatWarning" << std::endl;
}
void LeisaMdSpi::OnRspError(CThostFtdcRspInfoField *infoField, int requestID, bool isLast) {
	bool result = infoField && infoField->ErrorID;
	if (!result) {
		std::cout << "SuccessRspError" << std::endl;
	}
	else {
		std::cerr << "RspErrorFail" << std::endl;
	}
}
void LeisaMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *loginField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast) {
	bool result = infoField && (infoField->ErrorID!=0);
	if (!result) {
		std::cout << "LoginSuccess" << std::endl;
		std::cout << "�����գ� " << loginField->TradingDay << std::endl;
		std::cout << "��¼ʱ�䣺 " << loginField->LoginTime << std::endl;
		std::cout << "�����̣� " << loginField->BrokerID << std::endl;
		std::cout << "�ʻ����� " << loginField->UserID << std::endl;
		int result = mdApi->SubscribeMarketData(mdInstrumentID,instrumentNum);
		if (!result) {
			std::cout << "SubscribeMarketDataSuccess!" << std::endl;
		}
		else {
			std::cerr << "SubcribeMarketDataFail!" << std::endl;
		}
	}
	else {
		std::cerr << "RspLoginError" << std::endl;
	}
}

void LeisaMdSpi::OnRspUserLogOut(CThostFtdcUserLogoutField *logoutField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast) {
	bool result= infoField && infoField->ErrorID;
	if (!result) {
		std::cout << "LogoutSuccess" << std::endl;
	}
	else {
		std::cerr << "RspLogoutError" << std::endl;
	}
}


void LeisaMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast) {
	bool result = infoField && infoField->ErrorID;
	if (!result) {
		std::cout << "SubMarketDataSuccess" << std::endl;
		std::cout << "��Լ���룺 " << instrumentField->InstrumentID << std::endl;
		char filePath[100] = { '\0' };
		sprintf_s(filePath, "%s_market_data.csv", instrumentField->InstrumentID);
		std::ofstream outFile;
		outFile.open(filePath, std::ios::out);
		outFile << "��Լ����" << ","
			<< "����ʱ��" << ","
			<< "���¼�" << ","
			<< "�ɽ���" << ","
			<< "���һ" << ","
			<< "����һ" << ","
			<< "����һ" << ","
			<< "����һ" << ","
			<< "�ֲ���" << ","
			<< "������"
			<< std::endl;
		outFile.close();
	}
	else {
		std::cerr << "SubMarketDataFail!" << std::endl;
	}
}

void LeisaMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast) {
	bool result = infoField && infoField->ErrorID;
	if (!result) {
		std::cout << "UnSubMarketDataSuccess" << std::endl;
		std::cout << "InstrumentID:" << instrumentField->InstrumentID << std::endl;
	}
	else {
		std::cerr << "UnSubMarketDataFail" << std::endl;
	}
}

void LeisaMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast) {
	bool result = infoField && infoField->ErrorID;
	if (!result) {
		std::cout << "SubQuoteSuccess" << std::endl;
		std::cout << "InstrumentID:" << instrumentField->InstrumentID << std::endl;
	}
	else {
		std::cerr << "SubQuoteFail" << std::endl;
	}
}

void LeisaMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast) {
	bool result = infoField && infoField->ErrorID;
	if (!result) {
		std::cout << "UnSubQuoteSuccess" << std::endl;
		std::cout << "InstrumentID:" << instrumentField->InstrumentID << std::endl;
	}
	else {
		std::cerr << "UnSubQuoteFail" << std::endl;
	}
}

void LeisaMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *depthMarketDataField) {
	std::cout << "=====����������=====" << std::endl;
	std::cout << "�����գ� " << depthMarketDataField->TradingDay << std::endl;
	std::cout << "���������룺 " << depthMarketDataField->ExchangeID << std::endl;
	std::cout << "��Լ���룺 " << depthMarketDataField->InstrumentID << std::endl;
	std::cout << "��Լ�ڽ������Ĵ��룺 " << depthMarketDataField->ExchangeInstID << std::endl;
	std::cout << "���¼ۣ� " << depthMarketDataField->LastPrice << std::endl;
	std::cout << "������ " << depthMarketDataField->Volume << std::endl;
	char filePath[100] = { '\0' };
	sprintf_s(filePath, "%s_market_data.csv", depthMarketDataField->InstrumentID);
	std::ofstream outFile;
	outFile.open(filePath, std::ios::app); // �ļ�׷��д�� 
	outFile << depthMarketDataField->InstrumentID << ","
		<< depthMarketDataField->UpdateTime << "." << depthMarketDataField->UpdateMillisec << ","
		<< depthMarketDataField->LastPrice << ","
		<< depthMarketDataField->Volume << ","
		<< depthMarketDataField->BidPrice1 << ","
		<< depthMarketDataField->BidVolume1 << ","
		<< depthMarketDataField->AskPrice1 << ","
		<< depthMarketDataField->AskVolume1 << ","
		<< depthMarketDataField->OpenInterest << ","
		<< depthMarketDataField->Turnover << std::endl;
	outFile.close();
}

void LeisaMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *quoteField) {
	std::cout << "=====���ѯ�۽��=====" << std::endl;
	std::cout << "�����գ� " << quoteField->TradingDay << std::endl;
	std::cout << "���������룺 " << quoteField->ExchangeID << std::endl;
	std::cout << "��Լ���룺 " << quoteField->InstrumentID << std::endl;
	std::cout << "ѯ�۱�ţ� " << quoteField->ForQuoteSysID << std::endl;
}