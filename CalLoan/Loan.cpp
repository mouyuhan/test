#include "Loan.h"

void LOAN::INI(std::string RepaymentType, double AnnualInterestRate, int RepaymentTerm, double Principal)
{
	//还款模式初始化
	RepaymentType0 = RepaymentType;
	//年利率初始化
	AnnualInterestRate0 = AnnualInterestRate;
	//月利率初始化
	MonthlyInterestRate0 = AnnualInterestRate0 / 12.0;
	//还款期初初始化
	RepaymentTerm0 = RepaymentTerm;
	//贷款本金初始化
	Principal0 = Principal;
}

void LOAN::AddPrerepayment(PrerepaymentInfo RepaymentInfo)
{
	RepaymentInfoData.push_back(RepaymentInfo);
}

void LOAN::ClearAllPrerepayment()
{
	RepaymentInfoData.clear();
}

void LOAN::CalLoan(std::string FileName)
{
	if (RepaymentInfoData.size() == 0)
	{
		//不进行提前还款
		if (RepaymentType0 == "等额本息")
		{
			CalAnnuity(Principal0, RepaymentTerm0, MonthlyInterestRate0,
				Result_RemainingPrincipal, Result_MonthlyRepaymentAmount,
				Result_MonthlyRepaymentInterst, Result_MonthlyRepaymentPrincipal);

			Result_TotalInterest = 0;
			for (size_t i = 0; i < Result_MonthlyRepaymentInterst.size(); i++)
			{
				Result_TotalInterest += Result_MonthlyRepaymentInterst[i];
			}
			Result_TotalAmount = Principal0 + Result_TotalInterest;

			std::cout<< "*************************\n";
			std::cout << "还款类型：等额本息\n" << "不提前还贷\n";
			std::cout << "还款总金额：" << std::fixed << std::setprecision(2) << Result_TotalAmount << std::endl;
			std::cout << "还款利息金额：" << std::fixed << std::setprecision(2) << Result_TotalInterest << std::endl;

		}
		else
		{
			CalLinear(Principal0, RepaymentTerm0, MonthlyInterestRate0,
				Result_RemainingPrincipal, Result_MonthlyRepaymentAmount,
				Result_MonthlyRepaymentInterst, Result_MonthlyRepaymentPrincipal);
		
			Result_TotalInterest = 0;
			for (size_t i = 0; i < Result_MonthlyRepaymentInterst.size(); i++)
			{
				Result_TotalInterest += Result_MonthlyRepaymentInterst[i];
			}
			Result_TotalAmount = Principal0 + Result_TotalInterest;

			std::cout << "*************************\n";
			std::cout << "还款类型：等额本金\n" << "不提前还贷\n";
			std::cout << "还款总金额：" << std::fixed << std::setprecision(2) << Result_TotalAmount << std::endl;
			std::cout << "还款利息金额：" << std::fixed << std::setprecision(2) << Result_TotalInterest << std::endl;		
		}
	}
	else
	{
		//进行提前还款
		if (RepaymentType0 == "等额本息")
		{
			//获得提前还款的次数
			int NumPrerepayment = RepaymentInfoData.size();
			//提前还款后的总期数
			int TotalRepaymentTerm = RepaymentTerm0;

			std::vector<double> temp_RemainingPrincipal;
			std::vector<double> temp_MonthlyRepaymentAmount;
			std::vector<double> temp_MonthlyRepaymentInterst;
			std::vector<double> temp_MonthlyRepaymentPrincipal;

			int RepaymentTerm_left = RepaymentTerm0;//在进行第i-1次提前还款后的剩余还款期限（i-1=-1时对应初始期限）
			double Principal_left = Principal0;//在进行第i-1次提前还款后的剩余本金（i-1=-1时对应初始本金）

			for (size_t i = 0; i < NumPrerepayment + 1; i++)
			{
				if (i == NumPrerepayment)
				{
					//最后一次提前还款计算后，对剩余期限内的还款进行赋值
					for (size_t j = 0; j < RepaymentTerm_left; j++)
					{
						Result_RemainingPrincipal.push_back(temp_RemainingPrincipal[j]);
						Result_MonthlyRepaymentAmount.push_back(temp_MonthlyRepaymentAmount[j]);
						Result_MonthlyRepaymentInterst.push_back(temp_MonthlyRepaymentInterst[j]);
						Result_MonthlyRepaymentPrincipal.push_back(temp_MonthlyRepaymentPrincipal[j]);
					}
					break;
				}

				if (RepaymentInfoData[i].PrerepaymentType == "缩短还款期限")
				{
					//清除临时数组信息
					temp_RemainingPrincipal.clear();
					temp_MonthlyRepaymentAmount.clear();
					temp_MonthlyRepaymentInterst.clear();
					temp_MonthlyRepaymentPrincipal.clear();

					//计算第i-1次提前还款后的贷款信息
					CalAnnuity(Principal_left, RepaymentTerm_left, MonthlyInterestRate0,
						temp_RemainingPrincipal, temp_MonthlyRepaymentAmount,
						temp_MonthlyRepaymentInterst, temp_MonthlyRepaymentPrincipal);
					
					//两次还款间的间隔
					int GapTerm = RepaymentInfoData[i].PrerepaymentTerm - Result_MonthlyRepaymentAmount.size();
					//对第i-1次提前还款后到第i次提前还款的贷款信息进行更新（但剩余本金未减去提前还款金额）
					for (size_t j = 0; j < GapTerm; j++)
					{
						Result_RemainingPrincipal.push_back(temp_RemainingPrincipal[j]);
						Result_MonthlyRepaymentAmount.push_back(temp_MonthlyRepaymentAmount[j]);
						Result_MonthlyRepaymentInterst.push_back(temp_MonthlyRepaymentInterst[j]);
						Result_MonthlyRepaymentPrincipal.push_back(temp_MonthlyRepaymentPrincipal[j]);
					}
					//剩余本金减去提前还款项
					Result_RemainingPrincipal.back() = Result_RemainingPrincipal.back() - RepaymentInfoData[i].PrerepaymentAmount;
					Principal_left = Result_RemainingPrincipal.back();
					//原剩余还款期数
					int RepaymentTerm_left0 = TotalRepaymentTerm - Result_MonthlyRepaymentAmount.size();

					//计算指定还款金额的条件下，还需要的还款期限
					//缩短还款期限后，每月还款与当前还款的差
					double res = 0;
					double res_ = 9999999;
					//计算剩余期限
					for (size_t j = RepaymentTerm_left0; j > 0; j--)
					{
						temp_RemainingPrincipal.clear();
						temp_MonthlyRepaymentAmount.clear();
						temp_MonthlyRepaymentInterst.clear();
						temp_MonthlyRepaymentPrincipal.clear();

						CalAnnuity(Principal_left, j, MonthlyInterestRate0,
							temp_RemainingPrincipal, temp_MonthlyRepaymentAmount,
							temp_MonthlyRepaymentInterst, temp_MonthlyRepaymentPrincipal);

						res = fabs(temp_MonthlyRepaymentAmount.back() - Result_MonthlyRepaymentAmount.back());
						//当偏差增大，说明上一次计算出的还款期限正确
						if (res > res_)
						{
							RepaymentTerm_left = j + 1;
							break;
						}
						res_ = res;
					}

					temp_RemainingPrincipal.clear();
					temp_MonthlyRepaymentAmount.clear();
					temp_MonthlyRepaymentInterst.clear();
					temp_MonthlyRepaymentPrincipal.clear();

					CalAnnuity(Principal_left, RepaymentTerm_left, MonthlyInterestRate0,
						temp_RemainingPrincipal, temp_MonthlyRepaymentAmount,
						temp_MonthlyRepaymentInterst, temp_MonthlyRepaymentPrincipal);
				}
				else if(RepaymentInfoData[i].PrerepaymentType == "减少月供")
				{
					//清除临时数组信息
					temp_RemainingPrincipal.clear();
					temp_MonthlyRepaymentAmount.clear();
					temp_MonthlyRepaymentInterst.clear();
					temp_MonthlyRepaymentPrincipal.clear();

					//计算第i-1次提前还款后的贷款信息
					CalAnnuity(Principal_left, RepaymentTerm_left, MonthlyInterestRate0,
						temp_RemainingPrincipal, temp_MonthlyRepaymentAmount,
						temp_MonthlyRepaymentInterst, temp_MonthlyRepaymentPrincipal);

					//两次还款间的间隔
					int GapTerm = RepaymentInfoData[i].PrerepaymentTerm - Result_MonthlyRepaymentAmount.size();
					//对第i-1次提前还款后到第i次提前还款的贷款信息进行更新（但剩余本金未减去提前还款金额）
					for (size_t j = 0; j < GapTerm; j++)
					{
						Result_RemainingPrincipal.push_back(temp_RemainingPrincipal[j]);
						Result_MonthlyRepaymentAmount.push_back(temp_MonthlyRepaymentAmount[j]);
						Result_MonthlyRepaymentInterst.push_back(temp_MonthlyRepaymentInterst[j]);
						Result_MonthlyRepaymentPrincipal.push_back(temp_MonthlyRepaymentPrincipal[j]);
					}
					//剩余本金减去提前还款项
					Result_RemainingPrincipal.back() = Result_RemainingPrincipal.back() - RepaymentInfoData[i].PrerepaymentAmount;
					Principal_left = Result_RemainingPrincipal.back();
					//原剩余还款期数
					RepaymentTerm_left = TotalRepaymentTerm - Result_MonthlyRepaymentAmount.size();

					temp_RemainingPrincipal.clear();
					temp_MonthlyRepaymentAmount.clear();
					temp_MonthlyRepaymentInterst.clear();
					temp_MonthlyRepaymentPrincipal.clear();

					CalAnnuity(Principal_left, RepaymentTerm_left, MonthlyInterestRate0,
						temp_RemainingPrincipal, temp_MonthlyRepaymentAmount,
						temp_MonthlyRepaymentInterst, temp_MonthlyRepaymentPrincipal);
				}
				else
				{
					std::cout << "未设置提前还款方式！\n";
				}


			}
			Result_TotalInterest = 0;
			for (size_t i = 0; i < Result_MonthlyRepaymentInterst.size(); i++)
			{
				Result_TotalInterest += Result_MonthlyRepaymentInterst[i];
			}
			Result_TotalAmount = Principal0 + Result_TotalInterest;

			std::cout << "*************************\n";
			std::cout << "还款类型：等额本息\n" << "提前还贷\n";
			std::cout << "还款期数" << std::fixed << std::setprecision(2) << Result_MonthlyRepaymentAmount.size() << std::endl;
			std::cout << "还款总金额：" << std::fixed << std::setprecision(2) << Result_TotalAmount << std::endl;
			std::cout << "还款利息金额：" << std::fixed << std::setprecision(2) << Result_TotalInterest << std::endl;

		}
		else
		{

		}
	}

	//输出计算数据
	std::ofstream FileOut;
	FileOut.open(FileName);
	FileOut << "期数\t" << "月供金额\t" << "本金占款\t" << "利息占款\t" << "剩余本金\t" << std::endl;
	for (size_t i = 0; i < Result_RemainingPrincipal.size(); i++)
	{
		FileOut << std::fixed << std::setprecision(2) << i + 1 << '\t'
			<< std::fixed << std::setprecision(2) << Result_MonthlyRepaymentAmount[i] << '\t'
			<< std::fixed << std::setprecision(2) << Result_MonthlyRepaymentPrincipal[i] << '\t'
			<< std::fixed << std::setprecision(2) << Result_MonthlyRepaymentInterst[i] << '\t'
			<< std::fixed << std::setprecision(2) << Result_RemainingPrincipal[i] << '\t'
			<< std::endl;
	}

}

void LOAN::CalAnnuity(double Principal, int RepaymentTerm, double MonthlyInterestRate0,
	std::vector<double>& RemainingPrincipal, std::vector<double>& MonthlyRepaymentAmount,
	std::vector<double>& MonthlyRepaymentInterst, std::vector<double>& MonthlyRepaymentPrincipal)
{
	double temp_d;
	//计算每月还款总金额
	temp_d = Principal * MonthlyInterestRate0 * pow(1 + MonthlyInterestRate0, RepaymentTerm) / (pow((1 + MonthlyInterestRate0), RepaymentTerm) - 1);
	for (size_t i = 0; i < RepaymentTerm; i++)
	{
		MonthlyRepaymentAmount.push_back(temp_d);
	}
	//计算每月还款利息占款和剩余本金
	for (size_t i = 0; i < RepaymentTerm; i++)
	{
		//第一个月还款计算
		if (i == 0)
		{
			temp_d = Principal * MonthlyInterestRate0;
			MonthlyRepaymentInterst.push_back(temp_d);
			temp_d = MonthlyRepaymentAmount[i] - temp_d;
			MonthlyRepaymentPrincipal.push_back(temp_d);
			temp_d = Principal - MonthlyRepaymentPrincipal[i];
			RemainingPrincipal.push_back(temp_d);
		}
		else
		{
			temp_d = RemainingPrincipal[i-1] * MonthlyInterestRate0;
			MonthlyRepaymentInterst.push_back(temp_d);
			temp_d = MonthlyRepaymentAmount[i] - temp_d;
			MonthlyRepaymentPrincipal.push_back(temp_d);
			temp_d = RemainingPrincipal[i-1] - MonthlyRepaymentPrincipal[i];
			RemainingPrincipal.push_back(temp_d);
		}
	}
}

void LOAN::CalLinear(double Principal, int RepaymentTerm, double MonthlyInterestRate0,
	std::vector<double>& RemainingPrincipal, std::vector<double>& MonthlyRepaymentAmount,
	std::vector<double>& MonthlyRepaymentInterst, std::vector<double>& MonthlyRepaymentPrincipal)
{
	double temp_d;
	//计算每月还款本金金额
	temp_d = Principal / RepaymentTerm;
	for (size_t i = 0; i < RepaymentTerm; i++)
	{
		MonthlyRepaymentPrincipal.push_back(temp_d);
	}
	//计算每月还款利息占款和剩余本金
	for (size_t i = 0; i < RepaymentTerm; i++)
	{
		//第一个月还款计算
		if (i == 0)
		{
			temp_d = Principal * MonthlyInterestRate0;
			MonthlyRepaymentInterst.push_back(temp_d);
			temp_d = MonthlyRepaymentPrincipal[i] + MonthlyRepaymentInterst[i];
			MonthlyRepaymentAmount.push_back(temp_d);
			temp_d = Principal - MonthlyRepaymentPrincipal[i];
			RemainingPrincipal.push_back(temp_d);
		}
		else
		{
			temp_d = RemainingPrincipal[i - 1] * MonthlyInterestRate0;
			MonthlyRepaymentInterst.push_back(temp_d);
			temp_d = MonthlyRepaymentPrincipal[i] + MonthlyRepaymentInterst[i];
			MonthlyRepaymentAmount.push_back(temp_d);
			temp_d = RemainingPrincipal[i - 1] - MonthlyRepaymentPrincipal[i];
			RemainingPrincipal.push_back(temp_d);
		}

	}
}

