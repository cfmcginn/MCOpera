#include <iostream>
#include <time.h>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMath.h"

#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_01.hpp"

boost::random::mt19937 gen;

void genHist(const std::string fileName, const std::string histName);
void makeHistInt(const std::string fileName, const std::string histName);
float fitHistFull(const std::string fileName, const std::string histName, bool Mod);
void runPoisson();

int main(int argc, char*argv[])
{
  if(argc!=3){
    std::cout << "Usage: runOperaMC <nSim> <outName>" << std::endl;
    return 1;
  }

  //  gen.seed(static_cast<unsigned int>(std::time(0)));

  TH1F* delT_p = new TH1F("delT_h", "delT_h", 200, -500, 500);
  TH1F* delTMod_p = new TH1F("delTMod_h", "delTMod_h", 200, -500, 500);

  for(int iter = 0; iter < atoi(argv[1]); iter++){
    std::cout << iter << std::endl;
    genHist(argv[2], Form("simulation%d", iter));
    makeHistInt(argv[2], Form("simulation%d", iter));

    delT_p->Fill(fitHistFull(argv[2], Form("simulation%d", iter), false));
    delTMod_p->Fill(fitHistFull(argv[2], Form("simulation%d", iter), true));
  }

  TFile* outFile_p = new TFile(Form("%s.root", argv[2]), "UPDATE");
  delT_p->Write("", TObject::kOverwrite);
  delTMod_p->Write("", TObject::kOverwrite);
  outFile_p->Close();
  delete outFile_p;

  //  runPoisson();

  return 0;
}


void genHist(const std::string fileName, const std::string histName)
{
  // From Opera Paper                                                                                      
  
  const int nEvtsDetected = 15223;
  const float timeInt = 10500;
  const float timeIntExp = 11500;
  const int res = 100;
  
  TH1::SetDefaultSumw2();
  TH1F* testHist_p = new TH1F(Form("%s_h", histName.c_str()), Form("%s_h", histName.c_str()), 230, -timeIntExp/2.0, timeIntExp/2.0);
  
  while(testHist_p->Integral() < nEvtsDetected){  
    static boost::random::uniform_01<boost::mt19937> dist(gen);
    float randNum =  dist()*timeIntExp - timeIntExp/2.0;
    float weight = .5*(TMath::Erf((randNum + timeInt/2.0)/(TMath::Sqrt(2)*res)) - TMath::Erf((randNum - timeInt/2.0)/(TMath::Sqrt(2)*res)))/timeInt;
    testHist_p->Fill(randNum, weight*10000.);  
  }
  
  TFile* outFile_p = new TFile(Form("%s.root", fileName.c_str()), "UPDATE");
  testHist_p->SetMinimum(0.0001);
  testHist_p->Write("", TObject::kOverwrite);
  outFile_p->Close();
  delete outFile_p;
  delete testHist_p;
  
  return;
}


void makeHistInt(const std::string fileName, const std::string histName)
{
  TFile* outFile_p = new TFile(Form("%s.root", fileName.c_str()), "UPDATE");
  TH1F* getHist_p = (TH1F*)outFile_p->Get(Form("%s_h", histName.c_str()));

  for(int iter = 0; iter < getHist_p->GetNbinsX(); iter++){
    int binNum = TMath::Nint(getHist_p->GetBinContent(iter+1));
    getHist_p->SetBinContent(iter+1, binNum);
    getHist_p->SetBinError(iter+1, TMath::Sqrt((float)binNum));
  }
  getHist_p->Write("", TObject::kOverwrite);
  outFile_p->Close();
  delete outFile_p;

  return;
}


float fitHistFull(const std::string fileName, const std::string histName, bool Mod)
{
  const float timeInt = 11500;

  TFile* outFile_p = new TFile(Form("%s.root", fileName.c_str()), "UPDATE");
  TH1F* getHist_p = (TH1F*)outFile_p->Get(Form("%s_h", histName.c_str()));

  TF1 *fullFit = new TF1("fullFit", ".5*[0]*(TMath::Erf((x-[2])/(TMath::Sqrt(2)*100)) - TMath::Erf((x-[1])/(TMath::Sqrt(2)*100)))/([1] - [2])", -timeInt/2.0, timeInt/2.0);

  fullFit->SetParName(0, "Amp");
  fullFit->SetParName(1, "b");
  fullFit->SetParName(2, "a");

  fullFit->SetParameter(0, getHist_p->GetMaximum()*timeInt);
  fullFit->SetParameter(1, (timeInt-1000)/2.0);
  fullFit->SetParameter(2, -(timeInt-1000)/2.0);

  if(!Mod) getHist_p->Fit("fullFit", "RMLQ");
  else getHist_p->Fit("fullFit", "RMLQ", "", -timeInt/2.0, -timeInt/2.0 + 1500);

  getHist_p->Write("", TObject::kOverwrite);
  outFile_p->Close();
  delete outFile_p;

  if(!Mod) return (fullFit->GetParameter(1) + fullFit->GetParameter(2))/2.0;
  else return fullFit->GetParameter(2) + (timeInt-1000)/2.0;
}


void runPoisson()
{
  int means[7] = {1, 2, 5, 10, 20, 50, 100};

  for(int iter = 0; iter < 7; iter++){
    double integral = 0;
    int count = 0;

    std::cout << "For mean: " << means[iter] << std::endl;
    while(integral < 1-3*TMath::Power(10, -7)){
      integral += TMath::Power(means[iter], count)*TMath::Power(TMath::E(), -means[iter])/(TMath::Factorial(count));
      if(integral < .05) std::cout << "   95% count: " << count << ", "<< integral << std::endl;
      if(integral > .9973) std::cout << "   3sig: " << count << ", " << integral << std::endl;
      count++;
    }
    std::cout << "   5sig: " << count << ", " << integral << std::endl;
  }

  return;
}
