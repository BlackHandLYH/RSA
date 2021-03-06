// RSA.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include <ctime>
#include <cmath>
#include <vector>
#include <ctime>

//不同方法判断 输入保证大于等于3
//使用试除法 3~n
bool IsPrimeByDivision1(const unsigned long &num) {
	if (num % 2 == 0)		//如果是偶数直接拒绝
		return false;
	for (int i = 3; i < num; i += 2)
	{
		if (num % i == 0)
		{
			return false;
			break;
		}
	}
	return true;
}

//使用试除法 3~sqrt(n)
bool IsPrimeByDivision2(const unsigned long &num) {
	if (num % 2 == 0)		//如果是偶数直接拒绝
		return false;
	for (int i = 3; i * i <= num; i += 2)
	{
		if (num % i == 0)
		{
			return false;
			break;
		}
	}
	return true;
}

//使用试除法 质数 in 3~sqrt(n)
bool IsPrimeByDivision3(const unsigned long &num, std::vector<unsigned long> &Primes) {
	if (num % 2 == 0)		//如果是偶数直接拒绝
		return false;
	std::vector<unsigned long int>::iterator it = Primes.begin();
	for (; it != Primes.end(); it++)
	{
		if (num % (*it) == 0 && (*it)*(*it) <= num)
		{
			return false;
			break;
		}
	}
	return true;
}

//求取幂
unsigned long GetPow(unsigned long a, unsigned long b, unsigned long c)
{
	unsigned long ret = 1;
	while (b) {
		if (b & 1)ret *= a, ret %= c;
		a *= a;
		a %= c;
		b >>= 1;
	}
	return ret;
}

//使用概率法
bool IsPrimeByProb(const unsigned long int &num, const int &k) {
	bool flag = true;
	srand(rand()*rand());
	for (int i = 0; i < k; i++)
	{
		unsigned long int sampledNum = rand() % num + 2;
		if (num % sampledNum == 0)
			continue;
		if ((GetPow(sampledNum, (num - 1), num)) % num != 1)
			return false;
	}
	return flag;
}

//求出10^e_max以下的质数表
//method 为选择方法的取值
//目前	1为试除法
//		2为概率法
//		3为筛选法
void FindPrimes(std::vector<unsigned long int> &Primes, const int &max, const int &method, const int &k) {
	

	if (1 == method)
	{
		Primes.push_back(2);
		for (unsigned long int i = 3; i < max; i += 2)
		{
			if (IsPrimeByDivision2(i))
				Primes.push_back(i);
		}
	}
	else if (2 == method)
	{
		Primes.push_back(2);
		for (unsigned long int i = 3; i < max; i += 2)
		{
			if (IsPrimeByProb(i, k))
				Primes.push_back(i);
		}
	}
	else if (3 == method)
	{
		std::vector<unsigned long int> Nums;
		for (unsigned long int i = 2; i < max; i++)
		{
			Nums.push_back(i);
		}
		
		while (true)
		{
			std::vector<unsigned long int>::iterator it = Nums.begin();
			unsigned long min = *(Nums.begin());
			Primes.push_back(*it);
			it = Nums.erase(it);
			if (it == Nums.end())
				break;
			for (; it != Nums.end(); )
			{
				if (((*it) % min) == 0)
				{
					it = Nums.erase(it);
					continue;
				}
				it++;
			}
		}
	}
	else
	{
		std::cout << "暂无该方法。" << std::endl;
	}
}

//不同试除法比较
void CompareDivision(std::vector<unsigned long int> &Primes, const int &e_max, const int &method) {
	unsigned long int max = (unsigned long int)pow(10, e_max);
	Primes.push_back(2);

	switch (method)
	{
	case 1://3~n
	{
		for (unsigned long int i = 3; i < max; i += 2)
		{
			if (IsPrimeByDivision1(i))
				Primes.push_back(i);
		}
		break;
	}
	case 2:
	{
		for (unsigned long int i = 3; i < max; i += 2)
		{
			if (IsPrimeByDivision2(i))
				Primes.push_back(i);
		}
		break;
	}
	case 3:
	{
		for (unsigned long int i = 3; i < max; i += 2)
		{
			if (IsPrimeByDivision3(i, Primes))
				Primes.push_back(i);
		}
		break;
	}
	default:
		break;
	}
}

//直接找出第N个质数
unsigned long FindNthPrime(const unsigned long &N)
{
	unsigned long a = (unsigned long) (N * log(N));
	unsigned long epsilon = (unsigned long)(0.1 * a);

	for (unsigned long i = a - epsilon; i <= a + epsilon; i++)
	{
		if (i % 2 == 0)
			continue;
		else
			if (IsPrimeByDivision2(i))
				return i;
	}

	return 0;
}

//质数分解
bool PrimeDecom(const unsigned long &Num, unsigned long &Factor1, unsigned long &Factor2)
{
	std::vector<unsigned long int> Primes;
	FindPrimes(Primes, Num - 1, 1, 0);

	std::vector<unsigned long int>::iterator it = Primes.begin();
	while ((*it) * (*it) < Num)
	{
		if (Num % (*it) == 0)
		{
			Factor1 = *it;
			Factor2 = Num / Factor1;
			return true;
		}
		it++;
	}
	return false;
}

int main()
{
	char key;
	while (true)
	{
		int e_max;		//十的次方数

		std::cout << "请输入选项" << std::endl;
		std::cout << "1：不同方法求取素数表并验证概率法结果" << std::endl;
		std::cout << "2：不同试除法的计算复杂度及运行时间比较" << std::endl;
		std::cout << "3：直接求取第N个质数" << std::endl;
		std::cout << "4：双质因数分解" << std::endl;
		std::cin >> key;
		switch (key)
		{
		case '1':
		{
			std::cout << "请输入一个数：";
			std::cin >> e_max;		//输入
			unsigned long int max = (unsigned long int)pow(10, e_max);

			std::vector<unsigned long int> PrimesByDivision;	//试除法 记录所有素数
			std::vector<unsigned long int> PrimesByFilter;		//筛选法 记录所有素数

			FindPrimes(PrimesByDivision, max, 1, 0);			//使用试除法得到素数表
			FindPrimes(PrimesByFilter, max, 3, 0);			//使用筛选法得到素数表

			std::cout << "使用试除法得到：在10的" << e_max << "次方以下，共有" << PrimesByDivision.size() << "个素数" << std::endl;
			std::cout << "使用筛选法得到：在10的" << e_max << "次方以下，共有" << PrimesByFilter.size() << "个素数" << std::endl;
			std::cout << "使用概率法" << std::endl;
			for (int k = 1; k < 21; k++)
			{
				int Counter = 0;
				for (int j = 0; j < 10; j++)
				{
					std::vector<unsigned long int> PrimesByProb; //记录某一k下概率法求出的素数表
					FindPrimes(PrimesByProb, e_max, 2, k);
					if (PrimesByProb.size() != PrimesByDivision.size())
						Counter++;
				}
				std::cout << "k = " << k << "时，计算10次，错误了" << Counter << "次" << std::endl;
			}
			continue;
			break;
		}
		case '2':
		{
			std::cout << "请输入一个数：";
			std::cin >> e_max;		//输入
			unsigned long int max = (unsigned long int)pow(10, e_max);

			std::vector<unsigned long int> PrimesByDivision1, PrimesByDivision2, PrimesByDivision3;
			clock_t start, finish;
			//使用3~n
			start = clock();
			CompareDivision(PrimesByDivision1, max, 1);
			finish = clock();
			double time1 = (double)(finish - start);
			std::cout << "在10的" <<e_max<<"次方下，共有"<< PrimesByDivision1.size() << "个质数" << std::endl;
			std::cout << "试除3~n 所花的时间为" << time1 << "ms" << std::endl;

			//使用3~sqrt(n)
			start = clock();
			CompareDivision(PrimesByDivision2, max, 2);
			finish = clock();
			double time2 = (double)(finish - start);
			std::cout << "试除3~√n 所花的时间为" << time2 << "ms" << std::endl;

			//使用质数 in 3~sqrt(n)
			start = clock();
			CompareDivision(PrimesByDivision3, max, 3);
			finish = clock();
			double time3 = (double)(finish - start);
			std::cout << "试除3~√n中的质数所花的时间为" << time3 << "ms" << std::endl;
			break;

		}
		case '3':
		{
			unsigned long N = 0;
			std::cout << "请输入一个数N，将找到第N个质数" << std::endl;
			while (N <= 0)
			{
				std::cin >> N;
				if (N > 0)
					break;
				std::cout << "请输入一个正数！" << std::endl;
			}

			unsigned long prime = FindNthPrime(N);
			if (prime != 0)
			{
				std::cout << "第"<<N<<"个质数是" << prime << std::endl;
			}
			else
			{
				std::cout << "未找到第" << N <<"个质数"<< std::endl;
			}
			break;
		}
		case '4':
		{
			unsigned long Num;
			unsigned long Factor1 = 0, Factor2 = 0;
			std::cout << "请输入一个待分解的数" << std::endl;
			std::cin >> Num;

			if (PrimeDecom(Num, Factor1, Factor2))
			{
				std::cout << Num << "=" << Factor1 << "*" << Factor2 << std::endl;
			}
			else
				std::cout << Num << "不能进行双质数分解" << std::endl;

			break;
		}
		default:
			continue;
			break;
		}
		std::cout << std::endl;
	}

	system("pause");
	return 0;
}

