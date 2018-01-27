#ifndef HESTON_HPP
#define HESTON_HPP

#include<algorithm>
#include<iostream>
#include<vector>
#include"Option.hpp"
#include<cmath>
using namespace std;

class Heston
{
private:
	double Kappa;
	double Theta;
	double Epslon;
	double rho;
	Option* Opt;

public:
	// Constructor and destructor
	Heston() {};
	Heston(const double& kap, const double& ta, const double& eps, const double& ro, Option*& op);
	virtual ~Heston();

	// Calculate the volatility path
	void GetVolPath(const vector<double>& DW2, vector<double>& v_t);
	// Calculate the asset price path
	void GetUnderlyingPath(const vector<double>& DW1, const vector<double>& v_t,
		vector<double>& S_t);

};

#endif 