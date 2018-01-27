#include"Heston.hpp"
#include"NormalGenerator.hpp"
#include"Extra.hpp"
#include"MonteCarlo.hpp"

int main()
{
	// Timer
	clock_t start, end; // typedef long clock_t
	start = clock();
	// Input the number of paths and number of increments
	int NumPath = 1000, NumIncre = 10000;

	// Correlation of underlying and volatility
	double rho = -0.6;
	// Option type
	string OptType = "Call";
	// Inputs for the option pricing simulation
	double S_0 = 50.0;    // Initial spot price
	double K = 50.0;      // Strike price
	double r = 0.03;     // Risk-free rate
	double v_0 = 0.30; // Initial volatility 
	double T = 1.00;       // One year until expiry

	double Kappa = 1.15;   // Mean-reversion rate
	double Theta = 0.20;  // Long run average volatility
	double Epslon = 0.39;      // Vol of vol

	//Number of threads for multithreading
	int NumThreads = 4;

	// Option pricing to compare time spending
	double OptionPriceMC = MonteCarlo(NumPath, NumIncre, rho, S_0, K, r, v_0, T, 
		Kappa, Theta, Epslon, OptType);
	cout << "Ordinary MC yields:" << OptionPriceMC << endl;

	double OptionPriceMultiThread = MonteCarloMultiThread(NumThreads, NumPath, NumIncre, rho, S_0, K, r, v_0, T, 
		Kappa, Theta, Epslon, OptType);
	cout << "MultiThread MC yields:" << OptionPriceMultiThread << endl;

	double OptionPriceAnti = MonteCarloAntitheticVariates(NumPath, NumIncre, rho, S_0, K, r, v_0, T, 
		Kappa, Theta, Epslon, OptType);
	cout << "Antithetic MC yields:" << OptionPriceAnti << endl;

	// Test for effciency of ordinary vs antithetic
	int TrailNum = 100;
	vector<double> OptionPricesMC;
	vector<double> OptionPricesAnti;
	double TempPriceMC = 0, TempPriceAnti = 0;
	for (int i = 0; i < TrailNum; ++i)
	{
		
		TempPriceMC = MonteCarlo(NumPath, NumIncre, rho, S_0, K, r, v_0, T, Kappa, Theta, Epslon, OptType);
		TempPriceAnti = MonteCarloAntitheticVariates(NumPath, NumIncre, rho, S_0, K, r, v_0, T, Kappa, Theta, Epslon, OptType);
		cout << "Simulation " << i + 1 << " gives Ordinary MC as: " << TempPriceMC 
			<< " and Antithetic MC as: " << TempPriceAnti << endl;
		OptionPricesMC.push_back(TempPriceMC);
		OptionPricesAnti.push_back(TempPriceAnti);
	}

	// Calculate the variance of pricing
	double VarMC = CaclVariance(OptionPricesMC);
	double VarAnti = CaclVariance(OptionPricesAnti);
	cout << "Ordinary MC simulation has a standard deviation of: " << VarMC << endl;
	cout << "Antithetic MC simulation has a standard deviation of: " << VarAnti << endl;

	// Stop clock
	end = clock();
	double duration = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "\nMission Completed in " << duration << " seconds. " << endl;

	// Pause
	getchar();
	return 0;
}