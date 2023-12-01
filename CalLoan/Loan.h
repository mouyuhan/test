#pragma once
#ifndef LOAN_H_
#define LOAN_H_

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

struct PrerepaymentInfo
{
	std::string PrerepaymentType;//“缩短还款期限”或“减少月供”
	int PrerepaymentTerm;//进行提前还款的期数(从1开始),期数的含义是：在第i期还款后，立即再提前还贷
	double PrerepaymentAmount;//提前还款金额
};

class LOAN
{
private:
	//计算输入
	double AnnualInterestRate0;//初始年利率
	double MonthlyInterestRate0;//初始月利率
	double RepaymentTerm0;//初始还款月数（按月计算）
	double Principal0;//初始贷款本金
	std::string RepaymentType0;//初始还款模式，Linear为等额本金，Annuity为等额本息
	std::vector<PrerepaymentInfo> RepaymentInfoData;//提前还款信息
	
	//计算结果
	int Result_RepaymentTerm;//还款期数
	double Result_TotalInterest;//还款总利息
	double Result_TotalAmount;//还款总金额
	std::vector<double> Result_RemainingPrincipal;//剩余本金序列(进行第n个月还款后的剩余本金)
	std::vector<double> Result_MonthlyRepaymentAmount;//单月还款金额序列
	std::vector<double> Result_MonthlyRepaymentInterst;//单月还款利息占款
	std::vector<double> Result_MonthlyRepaymentPrincipal;//单月还款本金占款

public:
	//初始化一笔贷款
	void INI(std::string RepaymentType,double AnnualInterestRate,
		int RepaymentTerm, double Principal);
	//提前还款设置
	void AddPrerepayment(PrerepaymentInfo Repayment);//设置提前还款
	void ClearAllPrerepayment();//清除所有提前还款
	void CalLoan(std::string FileName);//计算贷款信息

	//等额本金计算
	void CalLinear(double Principal, int RepaymentTerm, double MonthlyInterestRate0,
		std::vector<double> &RemainingPrincipal, std::vector<double> &MonthlyRepaymentAmount,
		std::vector<double>& MonthlyRepaymentInterst, std::vector<double>& MonthlyRepaymentPrincipal);//
	//等额本息计算
	void CalAnnuity(double Principal, int RepaymentTerm, double MonthlyInterestRate0,
		std::vector<double>& RemainingPrincipal, std::vector<double>& MonthlyRepaymentAmount,
		std::vector<double>& MonthlyRepaymentInterst, std::vector<double>& MonthlyRepaymentPrincipal);


};




#endif // !LOAN_H_

