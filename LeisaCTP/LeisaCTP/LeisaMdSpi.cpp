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
	std::cerr << "ErrorCode： " << reason << std::endl;
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
		std::cout << "交易日： " << loginField->TradingDay << std::endl;
		std::cout << "登录时间： " << loginField->LoginTime << std::endl;
		std::cout << "经纪商： " << loginField->BrokerID << std::endl;
		std::cout << "帐户名： " << loginField->UserID << std::endl;
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
		std::cout << "合约代码： " << instrumentField->InstrumentID << std::endl;
		char filePath[100] = { '\0' };
		sprintf_s(filePath, "%s_market_data.csv", instrumentField->InstrumentID);
		std::ofstream outFile;
		outFile.open(filePath, std::ios::out);
		outFile << "合约代码" << ","
			<< "更新时间" << ","
			<< "最新价" << ","
			<< "成交量" << ","
			<< "买价一" << ","
			<< "买量一" << ","
			<< "卖价一" << ","
			<< "卖量一" << ","
			<< "持仓量" << ","
			<< "换手率"
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
	std::cout << "=====获得深度行情=====" << std::endl;
	std::cout << "交易日： " << depthMarketDataField->TradingDay << std::endl;
	std::cout << "交易所代码： " << depthMarketDataField->ExchangeID << std::endl;
	std::cout << "合约代码： " << depthMarketDataField->InstrumentID << std::endl;
	std::cout << "合约在交易所的代码： " << depthMarketDataField->ExchangeInstID << std::endl;
	std::cout << "最新价： " << depthMarketDataField->LastPrice << std::endl;
	std::cout << "数量： " << depthMarketDataField->Volume << std::endl;
	char filePath[100] = { '\0' };
	sprintf_s(filePath, "%s_market_data.csv", depthMarketDataField->InstrumentID);
	std::ofstream outFile;
	outFile.open(filePath, std::ios::app); // 文件追加写入 
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
	std::cout << "=====获得询价结果=====" << std::endl;
	std::cout << "交易日： " << quoteField->TradingDay << std::endl;
	std::cout << "交易所代码： " << quoteField->ExchangeID << std::endl;
	std::cout << "合约代码： " << quoteField->InstrumentID << std::endl;
	std::cout << "询价编号： " << quoteField->ForQuoteSysID << std::endl;
}