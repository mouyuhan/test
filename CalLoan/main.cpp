#include <iostream>
#include <cmath>
#include "Loan.h"


int main()
{
	//共进行五次提前还款
	const int NumPrm = 7;
	PrerepaymentInfo Prm1[NumPrm];
	PrerepaymentInfo Prm2[NumPrm];

	for (size_t i = 0; i < NumPrm; i++)
	{
		Prm1[i].PrerepaymentAmount = 200000;
		Prm1[i].PrerepaymentTerm = (i + 1) * 12;
		Prm1[i].PrerepaymentType = "减少月供";
	}

	for (size_t i = 0; i < NumPrm; i++)
	{
		Prm2[i].PrerepaymentAmount = 200000;
		Prm2[i].PrerepaymentTerm = (i + 1) * 12;
		Prm2[i].PrerepaymentType = "缩短还款期限";
	}

	LOAN test1,test2;

	test1.INI("等额本息", 0.0455, 240, 2400000);
	for (size_t i = 0; i < NumPrm; i++)
	{
		test1.AddPrerepayment(Prm1[i]);
	}
	test1.CalLoan("减小月供.txt");

	test2.INI("等额本息", 0.0455, 240, 2400000);
	for (size_t i = 0; i < NumPrm; i++)
	{
		test2.AddPrerepayment(Prm2[i]);
	}

	test2.CalLoan("缩短还款期限.txt");
	system("pause");
}

