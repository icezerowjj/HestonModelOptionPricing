#include"Heston.hpp"
// Consturctor and destructor
Heston::Heston(const double& kap, const double& ta, const double& eps, const double& ro, Option*& op)
{
	Kappa = kap;
	Theta = ta;
	Epslon = eps;
	rho = ro; 
	Opt = op;
}
Heston::~Heston() {}

// Function to get the volatility path 
void Heston::GetVolPath(const vector<double>& DW2, vector<double>& v_t)
{
	int NumIncre = DW2.size();
	// Get the length of increment
	double dt = Opt->T / static_cast<double>(NumIncre);
	// Start iterating from i = 1
	double VolMax = 0.0, VolTemp = 0.0;
	for (int i = 1; i < NumIncre; ++i)
	{
		VolMax = max(v_t[i - 1], 0.0);
		VolTemp = v_t[i - 1] + Kappa * dt * (Theta - VolMax) + Epslon * sqrt(VolMax * dt) * DW2[i - 1];
		v_t[i] = (VolTemp > 0) ? VolTemp : 0;
	}
	
}


// Calculate the asset price path
void Heston::GetUnderlyingPath(const vector<double>& DW1, const vector<double>& v_t,
	vector<double>& S_t)
{
	int NumIncre = DW1.size();
	// Get the length of increment
	double dt = Opt->T / static_cast<double>(NumIncre);
	double VolMax = 0.0;
	// Start iterating from i = 1
	for (int i = 1; i < NumIncre; ++i)
	{
		VolMax = max(v_t[i - 1], 0.0);
		S_t[i] = S_t[i - 1] * exp((Opt->r - 0.5 * VolMax) * dt + sqrt(VolMax * dt) * DW1[i - 1]);
	}
}
