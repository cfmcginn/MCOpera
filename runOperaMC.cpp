#include <iostream>

#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"

float getRandomFraction(int fracSize);

boost::random::mt19937 gen;

int main(int argc, char*argv[])
{
  if(argc!=2){
    std::cout << "Usage: runOperaMC <fracSize>" << std::endl;
    return 1;
  }

  std::cout << "Fraction: " << getRandomFraction(atoi(argv[1])) << std::endl;

  return 0;
}

float getRandomFraction(int fracSize)
{
  boost::random::uniform_int_distribution<> dist(0, fracSize);
  float fracNum = dist(gen);
  return fracNum/((float)fracSize);
}

