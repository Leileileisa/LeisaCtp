#include "pch.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
extern CThostFtdcTraderApi* traderApi;
extern TThostFtdcInvestorIDType investorID;
extern TThostFtdcBrokerIDType brokerID;
extern TThostFtdcPasswordType password;
extern TThostFtdcPasswordType newPassword;
extern int requestID;
TThostFtdcFrontIDType frontID;
TThostFtdcSessionIDType sessionID;
TThostFtdcOrderRefType orderRef;
const char* instrumentID = {"sp1906"};
void LeisaTraderSpi::OnFrontConnected() {
	std::cout << "OnFrontConnected" << std::endl;
	ReqUserLogin();
}
void LeisaTraderSpi::OnFrontDisconnected(int nReason) {
	std::cout << "OnFrontDisconnected" << std::endl;
}
void LeisaTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	bool result = pRspInfo && pRspInfo->ErrorID;
	if (!result) {
		std::cout << "PasswordUpdateSuccess!" << std::endl;
	}
	else {
		std::cerr << "PasswordUpdateFail!:" <<pRspInfo->ErrorMsg <<std::endl;
	}
}
void LeisaTraderSpi::ReqUserLogin() {
	CThostFtdcReqUserLoginField loginField;
	memset(&loginField, 0, sizeof(loginField));
	strcpy_s(loginField.BrokerID, brokerID);
	strcpy_s(loginField.UserID,investorID);
	strcpy_s(loginField.Password, password);
	int result=traderApi->ReqUserLogin(&loginField,requestID++);
	if (!result) {
		std::cout << "ReqUserLoginSuccess!" << std::endl;
	}else{
		std::cerr << "ReqUserLoginFail!" << std::endl;
	}
}
void LeisaTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!IsErrorRspInfo(pRspInfo)) { 
		std::cout << "OnRspUserLoginSuccess!" << std::endl; 
		frontID = pRspUserLogin->FrontID;
		sessionID = pRspUserLogin->SessionID;
		strcpy_s(orderRef,pRspUserLogin->MaxOrderRef);
		
		ReqQueryTraderAccount();
		//ReqOrderInsert();
		//ReqSettlementInfoConfirm();
		//ReqQryInvestorPositionCombineDetail();
		//ReqOrderAction();
	}
	else {
		std::cout << "OnRspUserLoginFail!" << std::endl;
	}
}
void LeisaTraderSpi::ReqQueryTraderAccount() {
	CThostFtdcQryTradingAccountField accountField;
	memset(&accountField, 0, sizeof(accountField));
	strcpy_s(accountField.BrokerID,brokerID);
	strcpy_s(accountField.InvestorID , investorID);
	std::this_thread::sleep_for(std::chrono::milliseconds(700));
	int result=traderApi->ReqQryTradingAccount(&accountField,requestID++);
	if (!result) {
		std::cout << "ReqQueryTraderAccoutSuccess" << std::endl;
	}
	else {
		std::cout << "ReqQueryTraderAccountFail:" << result<<std::endl;
	}
}
void LeisaTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!IsErrorRspInfo(pRspInfo)) {
		std::cout << "OnRspUserLogoutSuccess" << std::endl;
	}
	else {
		std::cout << "OnRspUserLogoutFail" << std::endl;
	}
}
void LeisaTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!IsErrorRspInfo(pRspInfo)) {
		std::cout << "OnRspQryTradingAccountSuccess!" << std::endl;
		std::cout << std::fixed;
		std::cout << std::setprecision(2);
		std::cout<<"可用资金为："<<(double)pTradingAccount->Available << std::endl;
	}
	else {
		std::cout << "OnRspQryTradingAccountFail!" << std::endl;
	}
}
void LeisaTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	IsErrorRspInfo(pRspInfo);
}
bool LeisaTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField* infoField) {
	bool result = infoField && infoField->ErrorID;
	if (!result) {

	}
	else {
		std::cerr << "errorMsg:" << infoField->ErrorMsg << std::endl;
	}
	return result;
}
void LeisaTraderSpi::ReqOrderInsert() {
	CThostFtdcInputOrderField orderField = { 0 };
	strcpy_s(orderField.BrokerID, brokerID);
	strcpy_s(orderField.InvestorID, investorID);
	strcpy_s(orderField.ExchangeID, "SHFE");
	strcpy_s(orderField.InstrumentID, instrumentID);
	strcpy_s(orderField.UserID,investorID);
	strcpy_s(orderField.OrderRef, orderRef);
	orderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	orderField.Direction = THOST_FTDC_D_Sell;
	orderField.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	orderField.CombHedgeFlag[0] = THOST_FTDC_ECIDT_Hedge;
	orderField.LimitPrice = 5052;
	orderField.VolumeTotalOriginal = 1;
	orderField.TimeCondition = THOST_FTDC_TC_GFD;
	orderField.VolumeCondition = THOST_FTDC_VC_AV;
	orderField.MinVolume = 1;
	orderField.ContingentCondition = THOST_FTDC_CC_Immediately;
	//orderField.StopPrice = 20500;
	orderField.UserForceClose = 0;
	orderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	orderField.IsAutoSuspend = 0;
	orderField.IsSwapOrder = 0;
	int result=traderApi->ReqOrderInsert(&orderField,requestID++);
	if (!result) {
		std::cout << "SuccessReqOrderInsert" << std::endl;
	}
	else {
		std::cout << "FailReqOrderInsert" << std::endl;
	}
}
void LeisaTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder){
	std::cout << "SuccessOnRtnOrder" <<pOrder->InsertTime<<","<<pOrder->FrontID<<","<<pOrder->SessionID<<","<<pOrder->OrderSysID<<","<<pOrder->OrderRef<<std::endl;
	std::cout << pOrder->VolumeTraded<<std::endl;
	std::cout << pOrder->OrderSubmitStatus << std::endl;
	std::cout << pOrder->OrderStatus << std::endl;
	std::cout << pOrder->StatusMsg << std::endl;
		//ReqOrderAction(pOrder->FrontID,pOrder->SessionID,pOrder->OrderRef);
	//std::cout << "SuccessOnRtnOrder" << pOrder->AccountID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ActiveTime << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ActiveTraderID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ActiveUserID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->BranchID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->BrokerID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->BrokerOrderSeq << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->BusinessUnit << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->CancelTime << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ClearingPartID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ClientID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->CombHedgeFlag << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->CombOffsetFlag << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ContingentCondition << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->CurrencyID<< std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->Direction << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ExchangeID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ExchangeInstID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->ForceCloseReason << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->FrontID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->GTDDate << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->InsertDate << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->InsertTime << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->InstallID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->InstrumentID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->InvestorID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->InvestUnitID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->IsAutoSuspend << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->IsSwapOrder << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->LimitPrice << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->MinVolume << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->NotifySequence << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->OrderLocalID << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->OrderPriceType << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->OrderSource << std::endl;
	////std::cout << "SuccessOnRtnOrder" << pOrder.o << std::endl;
	//std::cout << "SuccessOnRtnOrder" << pOrder->InsertTime << std::endl;
}
void LeisaTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	std::cout << "SuccessOnRtnTrade!" << std::endl;
	//ReqQueryTraderAccount();
}
void LeisaTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "SuccessOnRspOrderInsert:" <<pRspInfo->ErrorMsg<< std::endl;
}
void LeisaTraderSpi::ReqSettlementInfoConfirm() {
	CThostFtdcSettlementInfoConfirmField confirmField = { 0 };
	strcpy_s(confirmField.BrokerID,brokerID);
	strcpy_s(confirmField.InvestorID, investorID);
	int result=traderApi->ReqSettlementInfoConfirm(&confirmField, requestID++);
	if (!result) {
		std::cout << "SuccessReqSettlementInfoConfirm" << std::endl;
	}
	else {
		std::cout << "FailReqSettlementInfoConfirm" << std::endl;
	}
}
void LeisaTraderSpi::ReqQryInstrument() {
	CThostFtdcQryInstrumentField field = { 0 };
	//strcpy_s(field.ExchangeID,"SHFE");
	strcpy_s(field.InstrumentID,instrumentID);
	int result = traderApi->ReqQryInstrument(&field, requestID++);
	if (!result) {
		std::cout << "SuccessReqQryInstrument" << std::endl;
	}
	else {
		std::cout << "FailReqQryInstrument：" <<result<< std::endl;
	}
}
void LeisaTraderSpi::ReqQryInvestorPosition() {
	CThostFtdcQryInvestorPositionField field = { 0 };
	strcpy_s(field.BrokerID, brokerID);
	strcpy_s(field.InstrumentID, instrumentID);
	strcpy_s(field.InvestorID, investorID);
	std::this_thread::sleep_for(std::chrono::milliseconds(700));
	int result = traderApi->ReqQryInvestorPosition(&field, requestID++);
	if (!result) {
		std::cout << "SuccessReqQryInvestorPosition" << std::endl;
	}
	else {
		std::cout << "FailReqQryInvestorPosition" <<result<< std::endl;
	}
}
void LeisaTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!IsErrorRspInfo(pRspInfo)) {
		std::cout << "SuccessOnRspQryInstrument" << std::endl;
		ReqQryInvestorPosition();
		//ReqQueryTraderAccount();
	}
	else {
		std::cout << "FailOnRspQryInstrument:" << pRspInfo->ErrorMsg << std::endl;
	}
}
void LeisaTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!IsErrorRspInfo(pRspInfo)) {
		std::cout << "SuccessOnRspQryInvestorPosition" <<std::endl;
		ReqOrderInsert();
	}
	else {
		std::cout << "FailOnRspQryInvestorPosition" << std::endl;
	}
}
void LeisaTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!IsErrorRspInfo(pRspInfo)) {
		std::cout << "SuccessOnRspSettlementInfoConfirm" << std::endl;
		std::cout << pSettlementInfoConfirm->ConfirmDate << std::endl;
		std::cout << pSettlementInfoConfirm->ConfirmTime << std::endl;
		ReqQryInstrument();
	}
	else {
		std::cout << "FailOnRspSettlementInfoConfirm" << std::endl;
	}
}
bool LeisaTraderSpi::IsMyOrder(CThostFtdcOrderField* pOrder) {
	return true;
}
bool LeisaTraderSpi::IsTradeOrder(CThostFtdcOrderField* pOrder) {
	return true;
}
void LeisaTraderSpi::ReqOrderAction(TThostFtdcFrontIDType frontID, TThostFtdcSessionIDType sessionID, TThostFtdcOrderRefType orderRef) {
	CThostFtdcInputOrderActionField field = { 0 };
	strcpy_s(field.BrokerID, brokerID);
	strcpy_s(field.InvestorID, investorID);
	strcpy_s(field.UserID, investorID);
	strcpy_s(field.InstrumentID,instrumentID);
	strcpy_s(field.ExchangeID, "SHFE");
	field.FrontID = frontID;
	field.SessionID = sessionID;
	strcpy_s(field.OrderRef, orderRef);
	//strcpy_s(field.OrderSysID,pOrderSysIDType);
	field.ActionFlag = THOST_FTDC_AF_Delete;
	int result=traderApi->ReqOrderAction(&field,requestID++);
	if (!result) {
		std::cout << "ReqOrderActionSuccess" << std::endl;
	}
	else {
		std::cout << "ReqOrderActionFail" << std::endl;
	}
}
void LeisaTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!IsErrorRspInfo(pRspInfo)) {
		std::cout << "SuccessOnRspOrderAction:" << pInputOrderAction->SessionID << std::endl;
	}
	else {
		std::cout << "OnRspOrderAction:" << pRspInfo->ErrorMsg << std::endl;
	}
}
void LeisaTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {
	std::cout << "OnErrRtnOrderInsert" << std::endl;
}
void LeisaTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {
	//std::cout << "OnErrRtnOrderAction" << pOrderAction->FrontID<<","<<pOrderAction->SessionID<<","<<pOrderAction->OrderRef<<std::endl;
	//std::cout << pRspInfo->ErrorMsg << std::endl;
	//std::cout << "OnErrRtnOrderAction" <<pRspInfo->ErrorMsg << std::endl;
}
void LeisaTraderSpi::ReqQryInvestorPositionCombineDetail() {
	CThostFtdcQryInvestorPositionCombineDetailField field;
	strcpy_s(field.BrokerID,brokerID);
	strcpy_s(field.CombInstrumentID, instrumentID);
	strcpy_s(field.InvestorID, investorID);
	std::this_thread::sleep_for(std::chrono::milliseconds(700));
	int result=traderApi->ReqQryInvestorPositionCombineDetail(&field, requestID++);
	if (!result) {
		std::cout << "ReqQryInvestorPositionCombineDetailSuccess";
	}
	else {
		std::cout << "ReqQryInvestorPositionCombineDetailFail";
	}
}
void LeisaTraderSpi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!IsErrorRspInfo(pRspInfo)) {
		std::cout << "SuccessOnRspQryInvestorPositionCombineDetail:" << pInvestorPositionCombineDetail->TotalAmt<<std::endl;
	}
	else {
		std::cout << "FailOnRspQryInvestorPositionCombineDetail" << std::endl;
	}
}