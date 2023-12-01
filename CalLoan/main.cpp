#include <iostream>
#include <cmath>
#include "Loan.h"


int main()
{
	//�����������ǰ����
	const int NumPrm = 7;
	PrerepaymentInfo Prm1[NumPrm];
	PrerepaymentInfo Prm2[NumPrm];

	for (size_t i = 0; i < NumPrm; i++)
	{
		Prm1[i].PrerepaymentAmount = 200000;
		Prm1[i].PrerepaymentTerm = (i + 1) * 12;
		Prm1[i].PrerepaymentType = "�����¹�";
	}

	for (size_t i = 0; i < NumPrm; i++)
	{
		Prm2[i].PrerepaymentAmount = 200000;
		Prm2[i].PrerepaymentTerm = (i + 1) * 12;
		Prm2[i].PrerepaymentType = "���̻�������";
	}

	LOAN test1,test2;

	test1.INI("�ȶϢ", 0.0455, 240, 2400000);
	for (size_t i = 0; i < NumPrm; i++)
	{
		test1.AddPrerepayment(Prm1[i]);
	}
	test1.CalLoan("��С�¹�.txt");

	test2.INI("�ȶϢ", 0.0455, 240, 2400000);
	for (size_t i = 0; i < NumPrm; i++)
	{
		test2.AddPrerepayment(Prm2[i]);
	}

	test2.CalLoan("���̻�������.txt");
	system("pause");
}

