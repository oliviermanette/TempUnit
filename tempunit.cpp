#include "Arduino.h"
#include "tempunit.h"

TempUnit::TempUnit(){
  _currentDendriteSize = MAXSIZE;

  for (int i=0;i<MAXSIZE;i++){
    _dvalues[i]=0;
    _weights[i]=0;
    _std[i]=1;
    _k[i]=0;
  }
  _nb = 0;
  Serial.println("Hello from TempUnit ! ");
}

void TempUnit::setNewTU(float *fltVector){
  //Add all values to _dvalues
  int lTmpSize = getDendriteSize();

  for (int i=0;i<lTmpSize;i++)
    _dvalues[i]= fltVector[i];
}

int TempUnit::learnNewVector(float *fltVector, int lintReinforcement){
  int lTmpSize = getDendriteSize();
  float lfltTempScore, lfltTempNewK;
  _nb++;

  for (int i=0;i<lTmpSize;i++){
    lfltTempScore = unitScore(fltVector[i],_dvalues[i],_weights[i],_std[i]);
    lfltTempNewK = lintReinforcement * lfltTempScore/_nb;
    _weights[i] += lfltTempNewK;
    _dvalues[i] = (_k[i]*_dvalues[i]/(_k[i]+lfltTempNewK))+
    (lfltTempNewK/(_k[i]+lfltTempNewK))*fltVector[i];
    _k[i]+=lfltTempNewK;
    _std[i]+= lintReinforcement * (0.5 - lfltTempScore)/_nb;
  }
  {
    //float lfltSumStd = sum(&_std);
    float lfltSumWeight = sum(&_weights[0]);
    for (int i=0;i<lTmpSize;i++){
      //_std[i]/=lfltSumStd;
      _weights[i]/=lfltSumWeight;
    }
  }

  return _nb;
}

float TempUnit::sum(float *lfltValues){
  int lTmpSize = getDendriteSize();
  float lftOutput=0;
  for (int i=0;i<lTmpSize;i++)
    lftOutput += lfltValues[i];
  return lftOutput;
}

float TempUnit::getScore(float *fltVector){
  int lTmpSize = getDendriteSize();
  float lfltScore = 0;
  for (int i=0;i<lTmpSize;i++)
    lfltScore += unitScore(fltVector[i],_dvalues[i],_weights[i],_std[i]);
  return lfltScore;
}

float TempUnit::unitScore(float lfltInput, float lfltDendrite, float lfltWeight,
  float lfltSigma){

    return lfltWeight
              *
              (1/(lfltSigma*sqrt(2*_pi)))
              *
              (exp(-1*(lfltInput-lfltDendrite)/(2*lfltSigma*lfltSigma)));
  }

int TempUnit::setDendriteSize(int lintSize){
  if (lintSize<MAXSIZE)
    _currentDendriteSize = lintSize;
  else
    _currentDendriteSize = MAXSIZE;
  return _currentDendriteSize;
}

int TempUnit::getDendriteSize(){
  return _currentDendriteSize;
}

int getMaxDSize(){
  return MAXSIZE;
}
