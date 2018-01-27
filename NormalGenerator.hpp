// NormalGenerator.hpp

#ifndef NormalGenerator_HPP
#define NormalGenerator_HPP

// Import Boost library

#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

class NormalGenerator
{

public:

	// Empty at the moment
	virtual double getNormal() const = 0;
};


class BoostNormal : public NormalGenerator
{
private:

	boost::lagged_fibonacci607 rng;
	boost::normal_distribution<> nor;

	boost::variate_generator<boost::lagged_fibonacci607&, boost::normal_distribution<> >* myRandom;


public:
	BoostNormal();	// NB no uniform parameters

	// Implement (variant) hook function
	double getNormal() const;

	~BoostNormal();
};


#endif
