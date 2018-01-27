#include"Extra.hpp"

// Function to print the vector
void PrintData(vector<double>& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		cout << data[i] << endl;
	}
}

// Function to print the matrix
void PrintData(vector< vector<double> >& data)
{
	int rows = data.size();
	int cols = data[0].size();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cout << data[i][j] << " ";
		}
		cout << endl;
	}
}

// Function to do Cholesky decomposition
vector< vector<double> > Cholesky(vector< vector<double> >& data)
{
	int n = data.size();
	vector< vector<double> > mat(n, vector<double>(n));
	double sum1 = 0.0;
	double sum2 = 0.0;
	double sum3 = 0.0;
	// Initialize the first element
	mat[0][0] = sqrt(data[0][0]);

	// First elements of each row
	for (int j = 1; j <= n - 1; j++)
	{
		mat[j][0] = data[j][0] / mat[0][0];
	}	
	for (int i = 1; i <= (n - 2); i++)
	{
		for (int k = 0; k <= (i - 1); k++)
		{
			sum1 += pow(mat[i][k], 2);
		}	
		mat[i][i] = sqrt(data[i][i] - sum1);
		for (int j = (i + 1); j <= (n - 1); j++)
		{
			for (int k = 0; k <= (i - 1); k++)
			{
				sum2 += mat[j][k] * mat[i][k];
			}	
			mat[j][i] = (data[j][i] - sum2) / mat[i][i];
		}
	}
	for (int k = 0; k <= (n - 2); k++)
	{
		sum3 += pow(mat[n - 1][k], 2);
	}	
	mat[n - 1][n - 1] = sqrt(data[n - 1][n - 1] - sum3);

	return mat;
}

//Function to write matrix into csv file
void WriteToCsv(string CsvFile, vector< vector<double> >& data)
{
	ofstream myfile;
	myfile.open(CsvFile);
	int rows = data.size();
	int cols = data[0].size();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (j < cols - 1)
			{
				myfile << data[i][j] << ",";
			}
			else
			{
				myfile << data[i][j] << "\n";
			}
		}

	}
	myfile.close();
}

//Function to write vector into csv file
void WriteToCsv(string CsvFile, vector<double>& data)
{
	ofstream myfile;
	myfile.open(CsvFile);
	int rows = data.size();
	for (int i = 0; i < rows; i++)
	{
		myfile << data[i] << "\n";
	}
	myfile.close();
}

// Function to calculate standard deviation
double CaclVariance(vector<double>& data)
{
	double sum = std::accumulate(data.begin(), data.end(), 0.0);
	double mean = sum / data.size();
	double sq_sum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / data.size() - mean * mean);
	return stdev;
}