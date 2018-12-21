#pragma once
#include "../CTP_API/ThostFtdcMdApi.h"
class LeisaMdSpi :public CThostFtdcMdSpi {
public:
	//连接响应
	void OnFrontConnected();
	//断线响应
	void OnFrontDisConnected(int reason);
	//心跳超时警告
	void OnHeartBeatWarning(int timeLapse);
	//登陆响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *loginField,CThostFtdcRspInfoField *infoField,int requestID,bool isLast);
	//登出响应
	void OnRspUserLogOut(CThostFtdcUserLogoutField *logoutField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast);
	//错误应答
	void OnRspError(CThostFtdcRspInfoField *infoField, int requestID, bool isLast);
	//订阅行情应答
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *instrumentField,CThostFtdcRspInfoField *infoField,int requestID,bool isLast);
	//取消订阅行情应答
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField,int requestID, bool isLast);
	//订阅询价应答
	void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast);
	//取消订阅询价应答
	void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast);
	//深度行情通知
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *depthMarketDataField);
	//询价通知
	void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *quoteField);
};