#ifndef MONTECARLO_HPP
#define MONTECARLO_HPP

#include <thread>
#include"Heston.hpp"
#include"NormalGenerator.hpp"
#include"Extra.hpp"
using namespace std;

// Normal generator of a single normal random number
double GenNormal(boost::variate_generator<boost::mt19937, boost::normal_distribution<> > &generator);

// Normal generator of a normal random vector
void GenNormal(boost::variate_generator<boost::mt19937, boost::normal_distribution<> > &generator, std::vector<double> &res);

// Monte Carlo pricing method
double MonteCarlo(int NumPath, int NumIncre, double rho, double S_0, double K, double r, double v_0, double T,
	double Kappa, double Theta, double Epslon, string OptType);

// Monte Carlo pricing method with multithreading
void MonteCarloThread(int PerPaths, int NumIncre, double rho, double S_0, double K, double r, double v_0, double T,
	double Kappa, double Theta, double Epslon, string OptType, double& SumPayOff);

// Monte Carlo pricing method with multithreading
double MonteCarloMultiThread(int NumThreads, int NumPaths, int NumIncre, double rho, double S_0, double K, double r, double v_0, double T,
	double Kappa, double Theta, double Epslon, string OptType);

// Monte Carlo pricing method with Antithetic Variates
double MonteCarloAntitheticVariates(int NumPath, int NumIncre, double rho, double S_0, double K, double r, double v_0, double T,
	double Kappa, double Theta, double Epslon, string OptType);

#endif 