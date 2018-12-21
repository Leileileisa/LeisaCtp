#pragma once
#include "../CTP_API/ThostFtdcMdApi.h"
class LeisaMdSpi :public CThostFtdcMdSpi {
public:
	//������Ӧ
	void OnFrontConnected();
	//������Ӧ
	void OnFrontDisConnected(int reason);
	//������ʱ����
	void OnHeartBeatWarning(int timeLapse);
	//��½��Ӧ
	void OnRspUserLogin(CThostFtdcRspUserLoginField *loginField,CThostFtdcRspInfoField *infoField,int requestID,bool isLast);
	//�ǳ���Ӧ
	void OnRspUserLogOut(CThostFtdcUserLogoutField *logoutField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast);
	//����Ӧ��
	void OnRspError(CThostFtdcRspInfoField *infoField, int requestID, bool isLast);
	//��������Ӧ��
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *instrumentField,CThostFtdcRspInfoField *infoField,int requestID,bool isLast);
	//ȡ����������Ӧ��
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField,int requestID, bool isLast);
	//����ѯ��Ӧ��
	void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast);
	//ȡ������ѯ��Ӧ��
	void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *instrumentField, CThostFtdcRspInfoField *infoField, int requestID, bool isLast);
	//�������֪ͨ
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *depthMarketDataField);
	//ѯ��֪ͨ
	void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *quoteField);
};