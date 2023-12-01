#include "Loan.h"

void LOAN::INI(std::string RepaymentType, double AnnualInterestRate, int RepaymentTerm, double Principal)
{
	//����ģʽ��ʼ��
	RepaymentType0 = RepaymentType;
	//�����ʳ�ʼ��
	AnnualInterestRate0 = AnnualInterestRate;
	//�����ʳ�ʼ��
	MonthlyInterestRate0 = AnnualInterestRate0 / 12.0;
	//�����ڳ���ʼ��
	RepaymentTerm0 = RepaymentTerm;
	//������ʼ��
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
		//��������ǰ����
		if (RepaymentType0 == "�ȶϢ")
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
			std::cout << "�������ͣ��ȶϢ\n" << "����ǰ����\n";
			std::cout << "�����ܽ�" << std::fixed << std::setprecision(2) << Result_TotalAmount << std::endl;
			std::cout << "������Ϣ��" << std::fixed << std::setprecision(2) << Result_TotalInterest << std::endl;

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
			std::cout << "�������ͣ��ȶ��\n" << "����ǰ����\n";
			std::cout << "�����ܽ�" << std::fixed << std::setprecision(2) << Result_TotalAmount << std::endl;
			std::cout << "������Ϣ��" << std::fixed << std::setprecision(2) << Result_TotalInterest << std::endl;		
		}
	}
	else
	{
		//������ǰ����
		if (RepaymentType0 == "�ȶϢ")
		{
			//�����ǰ����Ĵ���
			int NumPrerepayment = RepaymentInfoData.size();
			//��ǰ������������
			int TotalRepaymentTerm = RepaymentTerm0;

			std::vector<double> temp_RemainingPrincipal;
			std::vector<double> temp_MonthlyRepaymentAmount;
			std::vector<double> temp_MonthlyRepaymentInterst;
			std::vector<double> temp_MonthlyRepaymentPrincipal;

			int RepaymentTerm_left = RepaymentTerm0;//�ڽ��е�i-1����ǰ������ʣ�໹�����ޣ�i-1=-1ʱ��Ӧ��ʼ���ޣ�
			double Principal_left = Principal0;//�ڽ��е�i-1����ǰ������ʣ�౾��i-1=-1ʱ��Ӧ��ʼ����

			for (size_t i = 0; i < NumPrerepayment + 1; i++)
			{
				if (i == NumPrerepayment)
				{
					//���һ����ǰ�������󣬶�ʣ�������ڵĻ�����и�ֵ
					for (size_t j = 0; j < RepaymentTerm_left; j++)
					{
						Result_RemainingPrincipal.push_back(temp_RemainingPrincipal[j]);
						Result_MonthlyRepaymentAmount.push_back(temp_MonthlyRepaymentAmount[j]);
						Result_MonthlyRepaymentInterst.push_back(temp_MonthlyRepaymentInterst[j]);
						Result_MonthlyRepaymentPrincipal.push_back(temp_MonthlyRepaymentPrincipal[j]);
					}
					break;
				}

				if (RepaymentInfoData[i].PrerepaymentType == "���̻�������")
				{
					//�����ʱ������Ϣ
					temp_RemainingPrincipal.clear();
					temp_MonthlyRepaymentAmount.clear();
					temp_MonthlyRepaymentInterst.clear();
					temp_MonthlyRepaymentPrincipal.clear();

					//�����i-1����ǰ�����Ĵ�����Ϣ
					CalAnnuity(Principal_left, RepaymentTerm_left, MonthlyInterestRate0,
						temp_RemainingPrincipal, temp_MonthlyRepaymentAmount,
						temp_MonthlyRepaymentInterst, temp_MonthlyRepaymentPrincipal);
					
					//���λ����ļ��
					int GapTerm = RepaymentInfoData[i].PrerepaymentTerm - Result_MonthlyRepaymentAmount.size();
					//�Ե�i-1����ǰ����󵽵�i����ǰ����Ĵ�����Ϣ���и��£���ʣ�౾��δ��ȥ��ǰ�����
					for (size_t j = 0; j < GapTerm; j++)
					{
						Result_RemainingPrincipal.push_back(temp_RemainingPrincipal[j]);
						Result_MonthlyRepaymentAmount.push_back(temp_MonthlyRepaymentAmount[j]);
						Result_MonthlyRepaymentInterst.push_back(temp_MonthlyRepaymentInterst[j]);
						Result_MonthlyRepaymentPrincipal.push_back(temp_MonthlyRepaymentPrincipal[j]);
					}
					//ʣ�౾���ȥ��ǰ������
					Result_RemainingPrincipal.back() = Result_RemainingPrincipal.back() - RepaymentInfoData[i].PrerepaymentAmount;
					Principal_left = Result_RemainingPrincipal.back();
					//ԭʣ�໹������
					int RepaymentTerm_left0 = TotalRepaymentTerm - Result_MonthlyRepaymentAmount.size();

					//����ָ��������������£�����Ҫ�Ļ�������
					//���̻������޺�ÿ�»����뵱ǰ����Ĳ�
					double res = 0;
					double res_ = 9999999;
					//����ʣ������
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
						//��ƫ������˵����һ�μ�����Ļ���������ȷ
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
				else if(RepaymentInfoData[i].PrerepaymentType == "�����¹�")
				{
					//�����ʱ������Ϣ
					temp_RemainingPrincipal.clear();
					temp_MonthlyRepaymentAmount.clear();
					temp_MonthlyRepaymentInterst.clear();
					temp_MonthlyRepaymentPrincipal.clear();

					//�����i-1����ǰ�����Ĵ�����Ϣ
					CalAnnuity(Principal_left, RepaymentTerm_left, MonthlyInterestRate0,
						temp_RemainingPrincipal, temp_MonthlyRepaymentAmount,
						temp_MonthlyRepaymentInterst, temp_MonthlyRepaymentPrincipal);

					//���λ����ļ��
					int GapTerm = RepaymentInfoData[i].PrerepaymentTerm - Result_MonthlyRepaymentAmount.size();
					//�Ե�i-1����ǰ����󵽵�i����ǰ����Ĵ�����Ϣ���и��£���ʣ�౾��δ��ȥ��ǰ�����
					for (size_t j = 0; j < GapTerm; j++)
					{
						Result_RemainingPrincipal.push_back(temp_RemainingPrincipal[j]);
						Result_MonthlyRepaymentAmount.push_back(temp_MonthlyRepaymentAmount[j]);
						Result_MonthlyRepaymentInterst.push_back(temp_MonthlyRepaymentInterst[j]);
						Result_MonthlyRepaymentPrincipal.push_back(temp_MonthlyRepaymentPrincipal[j]);
					}
					//ʣ�౾���ȥ��ǰ������
					Result_RemainingPrincipal.back() = Result_RemainingPrincipal.back() - RepaymentInfoData[i].PrerepaymentAmount;
					Principal_left = Result_RemainingPrincipal.back();
					//ԭʣ�໹������
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
					std::cout << "δ������ǰ���ʽ��\n";
				}


			}
			Result_TotalInterest = 0;
			for (size_t i = 0; i < Result_MonthlyRepaymentInterst.size(); i++)
			{
				Result_TotalInterest += Result_MonthlyRepaymentInterst[i];
			}
			Result_TotalAmount = Principal0 + Result_TotalInterest;

			std::cout << "*************************\n";
			std::cout << "�������ͣ��ȶϢ\n" << "��ǰ����\n";
			std::cout << "��������" << std::fixed << std::setprecision(2) << Result_MonthlyRepaymentAmount.size() << std::endl;
			std::cout << "�����ܽ�" << std::fixed << std::setprecision(2) << Result_TotalAmount << std::endl;
			std::cout << "������Ϣ��" << std::fixed << std::setprecision(2) << Result_TotalInterest << std::endl;

		}
		else
		{

		}
	}

	//�����������
	std::ofstream FileOut;
	FileOut.open(FileName);
	FileOut << "����\t" << "�¹����\t" << "����ռ��\t" << "��Ϣռ��\t" << "ʣ�౾��\t" << std::endl;
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
	//����ÿ�»����ܽ��
	temp_d = Principal * MonthlyInterestRate0 * pow(1 + MonthlyInterestRate0, RepaymentTerm) / (pow((1 + MonthlyInterestRate0), RepaymentTerm) - 1);
	for (size_t i = 0; i < RepaymentTerm; i++)
	{
		MonthlyRepaymentAmount.push_back(temp_d);
	}
	//����ÿ�»�����Ϣռ���ʣ�౾��
	for (size_t i = 0; i < RepaymentTerm; i++)
	{
		//��һ���»������
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
	//����ÿ�»������
	temp_d = Principal / RepaymentTerm;
	for (size_t i = 0; i < RepaymentTerm; i++)
	{
		MonthlyRepaymentPrincipal.push_back(temp_d);
	}
	//����ÿ�»�����Ϣռ���ʣ�౾��
	for (size_t i = 0; i < RepaymentTerm; i++)
	{
		//��һ���»������
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

