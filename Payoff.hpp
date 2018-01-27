#ifndef PAYOFF_HPP
#define PAYOFF_HPP

#include <algorithm>

class Payoff
{
public:
	// Constructor and destructor
	Payoff();
	virtual ~Payoff() {};
	// Pure virtual function
	virtual double operator() (const double& S) const = 0;

};

class PayoffCall:public Payoff
{
private:
	// Strike price
	double K;
public:
	// Constructor and destructor
	PayoffCall(const double& k);
	virtual ~PayoffCall() {};
	// Override
	virtual double operator() (const double& S) const;

};

class PayoffPut :public Payoff
{
private:
	// Strike price
	double K;
public:
	// Constructor and destructor
	PayoffPut(const double& k);
	virtual ~PayoffPut() {};
	// Override
	virtual double operator() (const double& S) const;

};

#endif 