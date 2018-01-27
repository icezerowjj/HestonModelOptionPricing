#ifndef OPTION_HPP
#define OPTION_HPP

#include"Payoff.hpp"

class Option
{
public:
	// Input parameters
	Payoff* pay_off;
	double K;
	double r;
	double T;
	// Constructor and destructor
	Option(double s_K, double s_r, double s_T, Payoff* s_pay_off);
	virtual ~Option();

};

#endif 