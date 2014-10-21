#include <iostream>
#include <time.h>

#include "TFile.h"
#include "TH1F.h"

#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"

float getRandomFraction(int fracSize);

boost::random::mt19937 gen;

int main(int argc, char*argv[])
{
  if(argc!=4){
    std::cout << "Usage: runOperaMC <fracSize> <nEvts> <outName>" << std::endl;
    return 1;
  }

  //  gen.seed(static_cast<unsigned int>(std::time(0)));

  //From Opera Paper
  const int nEvtsDetected = 15223;


  TH1F* testHist_p = new TH1F("testHist_h", "testHist_h", 10, -0.05, .95);

  for(int iter = 0; iter < atoi(argv[2]); iter++){
    float randNum = getRandomFraction(atoi(argv[1]));
    testHist_p->Fill(randNum);
  }

  TFile* outFile_p = new TFile(Form("%s.root", argv[3]), "UPDATE");
  TH1::SetDefaultSumw2();
  testHist_p->Scale(1./testHist_p->Integral());
  testHist_p->SetMaximum(.1999);
  testHist_p->SetMinimum(.0001);
  testHist_p->Write("", TObject::kOverwrite);
  outFile_p->Close();
  delete outFile_p;

  delete testHist_p;

  return 0;
}

float getRandomFraction(int fracSize)
{
  boost::random::uniform_int_distribution<> dist(0, fracSize);
  float fracNum = dist(gen);
  return fracNum/((float)(fracSize+1));
}

