#include "Arduino.h"
#include "tempunit.h"

TempUnit::TempUnit(){
  _currentDendriteSize = MAXSIZE;
  for (int i=0;i<MAXSIZE;i++){
    _dvalues[i]=0;
    _weights[i]=0;
    _std[i]=0;
  }
  _nb = 0;
}

void TempUnit::setNewTU(float *fltVector){
  //Add all values to _dvalues
  int lTmpSize = getDendriteSize();

  for (int i=0;i<lTmpSize;i++)
    _dvalues[i]= *fltVector[i];
}

float TempUnit::learnNewVector(float *fltVector){
  float lfltScore = getScore(fltVector);
   
  return 0;
}
float getScore(float *fltVector){
  int lTmpSize = getDendriteSize();
  float lfltScore = 0;
  for (int i=0;i<lTmpSize;i++){
    lfltScore += _weights
              *
              (1/(_std[i]*sqrt(2*_pi)))
              *
              (exp(-1*(fltVector[i]-_dvalues[i])/(2*_std[i]*_std[i])));
  }
  return lfltScore;
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
