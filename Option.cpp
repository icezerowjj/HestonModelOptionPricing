#include"Option.hpp"

Option::Option(double s_K, double s_r, double s_T, Payoff* s_pay_off)
{
	K = s_K;
	r = s_r;
	T = s_T;
	pay_off = s_pay_off;
}

Option::~Option() {}