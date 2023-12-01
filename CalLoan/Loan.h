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
	std::string PrerepaymentType;//�����̻������ޡ��򡰼����¹���
	int PrerepaymentTerm;//������ǰ���������(��1��ʼ),�����ĺ����ǣ��ڵ�i�ڻ������������ǰ����
	double PrerepaymentAmount;//��ǰ������
};

class LOAN
{
private:
	//��������
	double AnnualInterestRate0;//��ʼ������
	double MonthlyInterestRate0;//��ʼ������
	double RepaymentTerm0;//��ʼ�������������¼��㣩
	double Principal0;//��ʼ�����
	std::string RepaymentType0;//��ʼ����ģʽ��LinearΪ�ȶ��AnnuityΪ�ȶϢ
	std::vector<PrerepaymentInfo> RepaymentInfoData;//��ǰ������Ϣ
	
	//������
	int Result_RepaymentTerm;//��������
	double Result_TotalInterest;//��������Ϣ
	double Result_TotalAmount;//�����ܽ��
	std::vector<double> Result_RemainingPrincipal;//ʣ�౾������(���е�n���»�����ʣ�౾��)
	std::vector<double> Result_MonthlyRepaymentAmount;//���»���������
	std::vector<double> Result_MonthlyRepaymentInterst;//���»�����Ϣռ��
	std::vector<double> Result_MonthlyRepaymentPrincipal;//���»����ռ��

public:
	//��ʼ��һ�ʴ���
	void INI(std::string RepaymentType,double AnnualInterestRate,
		int RepaymentTerm, double Principal);
	//��ǰ��������
	void AddPrerepayment(PrerepaymentInfo Repayment);//������ǰ����
	void ClearAllPrerepayment();//���������ǰ����
	void CalLoan(std::string FileName);//���������Ϣ

	//�ȶ�����
	void CalLinear(double Principal, int RepaymentTerm, double MonthlyInterestRate0,
		std::vector<double> &RemainingPrincipal, std::vector<double> &MonthlyRepaymentAmount,
		std::vector<double>& MonthlyRepaymentInterst, std::vector<double>& MonthlyRepaymentPrincipal);//
	//�ȶϢ����
	void CalAnnuity(double Principal, int RepaymentTerm, double MonthlyInterestRate0,
		std::vector<double>& RemainingPrincipal, std::vector<double>& MonthlyRepaymentAmount,
		std::vector<double>& MonthlyRepaymentInterst, std::vector<double>& MonthlyRepaymentPrincipal);


};




#endif // !LOAN_H_

