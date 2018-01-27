#include"MonteCarlo.hpp"

// Normal generator of a single normal random number
double GenNormal(boost::variate_generator<boost::mt19937, boost::normal_distribution<> > &generator)
{
	return generator();
}

// Normal generator of a normal random vector
void GenNormal(boost::variate_generator<boost::mt19937, boost::normal_distribution<> > &generator, std::vector<double> &res)
{
	for (size_t i = 0; i<res.size(); ++i)
		res[i] = generator();
}

// Monte Carlo pricing method
double MonteCarlo(int NumPath, int NumIncre, double rho, double S_0, double K, double r, double v_0, double T,
	double Kappa, double Theta, double Epslon, string OptType)
{
	// Get corr matrix
	vector< vector<double> > Corr(2, vector<double>(2));
	Corr[0][0] = 1.0, Corr[0][1] = rho, Corr[1][0] = rho, Corr[1][1] = 1.0;
	// Cholesky decomposition of corr matrix
	vector< vector<double> > CorrMatrix = Cholesky(Corr);

	// Boost normal generator
	boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
		generator(boost::mt19937(time(0)), boost::normal_distribution<>());
	// Generate two uncorrelated vectors
	vector< vector<double> > rnd(NumIncre, vector<double>(2));
	// Generate two correlated vectors
	vector<double> DW1(NumIncre), DW2(NumIncre);
	// Create the PayOff, Option and Heston objects
	Payoff* PayOpt;
	if (OptType == "Call")
	{
		PayOpt = new PayoffCall(K);
	}
	else if (OptType == "Put")
	{
		PayOpt = new PayoffPut(K);
	}
	else
	{
		cout << "Invalid option type. Fail to execute." << endl;
		exit(0);
	}
	
	Option* Opt = new Option(K, r, T, PayOpt);
	Heston Hest(Kappa, Theta, Epslon, rho, Opt);

	// Monte Carlo simulation
	double SumPayOff = 0.0;
	// The final asset price
	double ST = 0.0;
	// Create the spot and vol paths
	vector<double> S_t(NumIncre, S_0);   // Vector of initial spot prices
	vector<double> v_t(NumIncre, v_0);   // Vector of initial vol prices

	// Create each path every time
	for (int s = 0; s < NumPath; ++s)
	{
		// Create random numbers
		for (int t = 0; t < NumIncre; ++t)
		{
			rnd[t][0] = GenNormal(generator);
			rnd[t][1] = GenNormal(generator);
			DW1[t] = rnd[t][0] * CorrMatrix[0][0] + rnd[t][1] * CorrMatrix[1][0];
			DW2[t] = rnd[t][0] * CorrMatrix[0][1] + rnd[t][1] * CorrMatrix[1][1];
		}
		Hest.GetVolPath(DW2, v_t);
		Hest.GetUnderlyingPath(DW1, v_t, S_t);
		// Calculate the final payoff of the call option
		ST = Opt->pay_off->operator()(S_t[NumIncre - 1]);
		SumPayOff += ST;
	}
	// Discounted payoff
	double OptionPrice = SumPayOff / (NumPath * exp(r * T));
	// Free memory
	delete Opt;
	delete PayOpt;
	return OptionPrice;
}

void MonteCarloThread(int PerPaths, int NumIncre, double rho, double S_0, double K, double r, double v_0, double T,
	double Kappa, double Theta, double Epslon, string OptType, double& SumPayOff)
{
	// Get corr matrix
	vector< vector<double> > Corr(2, vector<double>(2));
	Corr[0][0] = 1.0, Corr[0][1] = rho, Corr[1][0] = rho, Corr[1][1] = 1.0;
	// Cholesky decomposition of corr matrix
	vector< vector<double> > CorrMatrix = Cholesky(Corr);

	// Boost normal generator
	boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
		generator(boost::mt19937(time(0)), boost::normal_distribution<>());
	// Generate two uncorrelated vectors
	vector< vector<double> > rnd(NumIncre, vector<double>(2));
	// Generate two correlated vectors
	vector<double> DW1(NumIncre), DW2(NumIncre);
	// Create the PayOff, Option and Heston objects
	Payoff* PayOpt;
	if (OptType == "Call")
	{
		PayOpt = new PayoffCall(K);
	}
	else if (OptType == "Put")
	{
		PayOpt = new PayoffPut(K);
	}
	else
	{
		cout << "Invalid option type. Fail to execute." << endl;
		exit(0);
	}
	Option* Opt = new Option(K, r, T, PayOpt);
	Heston Hest(Kappa, Theta, Epslon, rho, Opt);

	// The final asset price
	double ST = 0.0;
	// Create the spot and vol paths
	vector<double> S_t(NumIncre, S_0);   // Vector of initial spot prices
	vector<double> v_t(NumIncre, v_0);   // Vector of initial vol prices

	for (int s = 0; s < PerPaths; ++s)
	{
		// Create random numbers
		for (int t = 0; t < NumIncre; ++t)
		{
			rnd[t][0] = GenNormal(generator);
			rnd[t][1] = GenNormal(generator);
			DW1[t] = rnd[t][0] * CorrMatrix[0][0] + rnd[t][1] * CorrMatrix[1][0];
			DW2[t] = rnd[t][0] * CorrMatrix[0][1] + rnd[t][1] * CorrMatrix[1][1];
		}
		Hest.GetVolPath(DW2, v_t);
		Hest.GetUnderlyingPath(DW1, v_t, S_t);
		// Calculate the final payoff of the call option
		auto temp = S_t[NumIncre - 1];
		ST = Opt->pay_off->operator()(S_t[NumIncre - 1]);
		SumPayOff += ST;
	}
	// Free memory
	delete Opt;
	delete PayOpt;
}

// Monte Carlo pricing method with multithreading
double MonteCarloMultiThread(int NumThreads, int NumPath, int NumIncre, double rho, double S_0, double K, double r, double v_0, double T,
	double Kappa, double Theta, double Epslon, string OptType)
{
	vector<thread> Threads(NumThreads);
	vector<double> SumValues(NumThreads);
	SumValues.resize(NumThreads);

	for (int i = 0; i < NumThreads; ++i)
	{
		int PerPaths = NumPath / NumThreads;
		if (i == 0) {
			PerPaths += NumPath % NumThreads; // get the remaining iterations calculated by thread 0
		}
		Threads.emplace_back(MonteCarloThread, PerPaths, NumIncre, rho, S_0, K, r, v_0, T,
			Kappa, Theta, Epslon, OptType, std::ref(SumValues[i]));
	}

	for (auto & thread : Threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
	// Calculate option price
	double OptionPrice = std::accumulate(SumValues.begin(), SumValues.end(), 0.0) / NumPath;
	return OptionPrice;
}

// Monte Carlo pricing method with Antithetic Variates
double MonteCarloAntitheticVariates(int NumPath, int NumIncre, double rho, double S_0, double K, double r, double v_0, double T,
	double Kappa, double Theta, double Epslon, string OptType)
{
	// Get corr matrix
	vector< vector<double> > Corr(2, vector<double>(2));
	Corr[0][0] = 1.0, Corr[0][1] = rho, Corr[1][0] = rho, Corr[1][1] = 1.0;
	// Cholesky decomposition of corr matrix
	vector< vector<double> > CorrMatrix = Cholesky(Corr);

	// Boost normal generator
	boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
		generator(boost::mt19937(time(0)),
			boost::normal_distribution<>());
	// Generate two uncorrelated vectors
	vector< vector<double> > rnd(NumIncre, vector<double>(2));
	// Generate two correlated vectors
	vector<double> DW1(NumIncre), DW2(NumIncre);
	vector<double> DW3(NumIncre), DW4(NumIncre);
	// Create the PayOff, Option and Heston objects
	Payoff* PayOpt;
	if (OptType == "Call")
	{
		PayOpt = new PayoffCall(K);
	}
	else if (OptType == "Put")
	{
		PayOpt = new PayoffPut(K);
	}
	else
	{
		cout << "Invalid option type. Fail to execute." << endl;
		exit(0);
	}
	Option* Opt = new Option(K, r, T, PayOpt);
	Heston Hest(Kappa, Theta, Epslon, rho, Opt);

	// Monte Carlo simulation
	double SumPayOff = 0.0;
	// The final asset price
	double ST1 = 0.0, ST2 = 0.0;
	// Create the spot and vol paths
	vector<double> S_t1(NumIncre, S_0);   // Vector of initial spot prices
	vector<double> v_t1(NumIncre, v_0);   // Vector of initial vol prices
	vector<double> S_t2(NumIncre, S_0);   // Vector of initial spot prices
	vector<double> v_t2(NumIncre, v_0);   // Vector of initial vol prices

										  // Create each path every time
	for (int s = 0; s < NumPath / 2; ++s)
	{
		// Create random numbers
		for (int t = 0; t < NumIncre; ++t)
		{
			rnd[t][0] = GenNormal(generator);
			rnd[t][1] = GenNormal(generator);
			DW1[t] = rnd[t][0] * CorrMatrix[0][0] + rnd[t][1] * CorrMatrix[1][0];
			DW2[t] = rnd[t][0] * CorrMatrix[0][1] + rnd[t][1] * CorrMatrix[1][1];
			DW3[t] = -rnd[t][0] * CorrMatrix[0][0] + -rnd[t][1] * CorrMatrix[1][0];
			DW4[t] = -rnd[t][0] * CorrMatrix[0][1] + -rnd[t][1] * CorrMatrix[1][1];
		}
		Hest.GetVolPath(DW2, v_t1);
		Hest.GetUnderlyingPath(DW1, v_t1, S_t1);
		Hest.GetVolPath(DW4, v_t2);
		Hest.GetUnderlyingPath(DW3, v_t2, S_t2);
		// Calculate the final payoff of the call option
		ST1 = Opt->pay_off->operator()(S_t1[NumIncre - 1]);
		ST2 = Opt->pay_off->operator()(S_t2[NumIncre - 1]);
		SumPayOff += ST1 + ST2;
	}
	// Discounted payoff
	double OptionPrice = SumPayOff / (NumPath * exp(r * T));
	// Free memory
	delete Opt;
	delete PayOpt;
	return OptionPrice;
}