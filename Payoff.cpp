#ifndef PAYOFF_CPP
#define PAYOFF_CPP

#include"Payoff.hpp"

Payoff::Payoff() {}

PayoffCall::PayoffCall(const double& k)
{
	K = k;
}

// Define the payoff functions for both put and call options
double PayoffCall::operator() (const double& S) const
{
	return std::max(S - K, 0.0);
}

PayoffPut::PayoffPut(const double& k)
{
	K = k;
}

// Define the payoff functions for both put and call options
double PayoffPut::operator() (const double& S) const
{
	return std::max(K - S, 0.0);
}

#endif