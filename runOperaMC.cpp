#include <iostream>
#include <time.h>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooFFTConvPdf.h"

#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_01.hpp"

boost::random::mt19937 gen;

void genHist(const std::string fileName, const std::string histName);

int main(int argc, char*argv[])
{
  if(argc!=2){
    std::cout << "Usage: runOperaMC <nSim> <outName>" << std::endl;
    return 1;
  }

  //  gen.seed(static_cast<unsigned int>(std::time(0)));

  for(int iter = 0; iter < atoi(argv[1]); iter++){
    genHist(argv[2], Form("simulation%d.root", iter));
  }

  return 0;
}


void genHist(const std::string fileName, const std::string histName)
{
  // From Opera Paper                                                                                                                                 
 const int nEvtsDetected = 15223;
 const int timeInt = 10500;
 const int res = 8;

 TH1::SetDefaultSumw2();
 TH1F* testHist_p = new TH1F(Form("%s_h", histName.c_str()), Form("%s_h", histName.c_str()), 100, 0, 10.5);
 
 while(testHist_p->Integral() < nEvtsDetected){
   static boost::random::uniform_01<boost::mt19937> dist(gen);
   float randNum =  dist()*10.5;
   float weight = (1/8.22495)*(.7+.2*TMath::Power(TMath::E(), -(randNum - 10.5/2)*(randNum - 10.5/2)/(2*(10.5/6)*(10.5/6))));
   testHist_p->Fill(randNum, weight);  
 }
 
 TFile* outFile_p = new TFile(Form("%s.root", fileName.c_str()), "UPDATE");
 testHist_p->SetMinimum(0.0001);
 testHist_p->Write("", TObject::kOverwrite);
 outFile_p->Close();
 delete outFile_p;
 delete testHist_p;
 
 return;
}
