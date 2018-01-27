#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<ctime>
#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include<fstream>  
#include<string>
#include <numeric>
using namespace std;
// Function to print the vector
void PrintData(vector<double>& data);
// Function to print the matrix
void PrintData(vector< vector<double> >& data);

// Function to do Cholesky decomposition
vector< vector<double> > Cholesky(vector< vector<double> >& data);

//Function to write matrix into csv file
void WriteToCsv(string CsvFile, vector< vector<double> >& data);

//Function to write vector into csv file
void WriteToCsv(string CsvFile, vector<double>& data);

// Function to calculate standard deviation
double CaclVariance(vector<double>& data);