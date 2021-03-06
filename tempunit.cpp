//#include "Arduino.h"
#include "tempunit.h"

TempUnit::TempUnit(){
  _currentDendriteSize = MAXSIZE;
  _identity = 255;
  _minScore = 1;
  _maxScore = 0;

  for (int i=0;i<MAXSIZE;i++){
    _dvalues[i]=0;
    _weights[i]=1;
    _std[i]=1;
    _k[i]=0;
  }
  _nb = 0;
}

bool TempUnit::setDValue(unsigned char luchrPosition, float lfltValue){
  if (luchrPosition<getDendriteSize()){
    _dvalues[luchrPosition] = lfltValue;
    return true;
  }
  else
    return false;
}

bool TempUnit::setWeight(unsigned char luchrPosition, float lfltValue){
  if (luchrPosition<getDendriteSize()){
    _weights[luchrPosition] = lfltValue;
    return true;
  }
  else
    return false;
}

bool TempUnit::setStd(unsigned char luchrPosition, float lfltValue){
  if (luchrPosition<getDendriteSize()){
    _std[luchrPosition] = lfltValue;
    return true;
  }
  else
    return false;
}

void TempUnit::setPoolID (unsigned char lchrIdent){
  //if (_identity==255)
    _identity = lchrIdent;
    Serial.print("Pool ID#:");
    Serial.println(getPoolID());
}

unsigned char TempUnit::getPoolID (){
  return _identity;
}

void TempUnit::setNewTU(float fltVector[]){
  //Add all values to _dvalues
  int lTmpSize = getDendriteSize();
  for (int i=0;i<lTmpSize;i++){
    _dvalues[i]= fltVector[i];
  }
}

int TempUnit::learnNewVector(float fltVector[], int lintReinforcement){
  int lTmpSize = getDendriteSize();
  float lfltTempScore, lfltTempNewK, lfltTmpKp;
  _nb++;

  for (int i=0;i<lTmpSize;i++){
    lfltTempScore = _rawScore(fltVector[i],_dvalues[i],_std[i]);
    _activationsDendritique[i]=lfltTempScore;
    lfltTempNewK = lintReinforcement * lfltTempScore/_nb;
    _weights[i] += lfltTempNewK * _nb / sqrt(_nb);
    if (_weights[i]<0)
      _weights[i]=0;
    lfltTmpKp = _k[i]+lfltTempNewK;
    float lfltMunm1 = _dvalues[i];
    if ((lintReinforcement>0)&&(lfltTmpKp))
        _dvalues[i] = (_k[i]*_dvalues[i]/lfltTmpKp)
        + (lfltTempNewK/lfltTmpKp)*fltVector[i];
    _k[i]+=lfltTempNewK;

    // if (_nb==2)
    //   _std[i]=0;
    // if (_nb>1){
    //   float lfltPhi = (_nb-1)*(pow(lfltMunm1,2)-pow(_dvalues[i],2));
    //   _std[i] = sqrt(((_nb-2)/(_nb-1))*(pow(_std[i],2)-lfltPhi) + pow((fltVector[i]-_dvalues[i]),2)/(_nb-1));
    // }
    _std[i]+= lintReinforcement * (0.5 - lfltTempScore)/sqrt(_nb);
    if (_std[i]<=0)
      _std[i]-= lintReinforcement * (0.5 - lfltTempScore)/sqrt(_nb);
  }
  normalizeAllWeights();
  return _nb;
}

void TempUnit::normalizeAllWeights(bool lblIsAbove1){
  int lTmpSize = getDendriteSize();
  float lfltSumWeight = sum(&_weights[0]);
  if (lblIsAbove1)
    if (lfltSumWeight<1){
      Serial.println(lfltSumWeight);
      return;
    }
  for (int i=0;i<lTmpSize;i++){
    if (lfltSumWeight>0)
      _weights[i]/=lfltSumWeight;
    else
      _weights[i]=1/lTmpSize;
  }
}

float TempUnit::getActivation(int lintDendritePosition){
  if (lintDendritePosition<getDendriteSize())
    return _activationsDendritique[lintDendritePosition];
  else
    return 0;
}

float TempUnit::getSynapseMean(int lIntPos){
  if (lIntPos<getDendriteSize())
    return _dvalues[lIntPos];
  else
    return 0;
}

float TempUnit::getSynapseStd(int lIntPos){
  if (lIntPos<getDendriteSize())
    return _std[lIntPos];
  else
    return 0;
}

float TempUnit::getSynapseWeight(int lIntPos){
  if (lIntPos<getDendriteSize())
    return _weights[lIntPos];
  else
    return 0;
}

void TempUnit::setDecWeightRatio(int lintIndex, float fltRatio){
    _weights[lintIndex] *= fltRatio;
}

float TempUnit::isSynapse(int lIntPos, float lfltInputValue){
    return _rawScore(lfltInputValue, _dvalues[lIntPos],_std[lIntPos]);
}

float TempUnit::sum(float *lfltValues){
  int lTmpSize = getDendriteSize();
  float lftOutput=0;
  for (int i=0;i<lTmpSize;i++)
    lftOutput += lfltValues[i];
  return lftOutput;
}

float TempUnit::getScore(float fltVector[]){
  int lTmpSize = getDendriteSize();
  float lfltScore = 0, lfltTempValue=0;
  for (int i=0;i<lTmpSize;i++){
    lfltTempValue = unitScore(fltVector[i],_dvalues[i],_weights[i],_std[i]);
    if (isnan(lfltTempValue))
      lfltTempValue = 0;
    lfltScore +=  lfltTempValue;
  }
  if (_minScore>lfltScore)
    _minScore = lfltScore;
  if (_maxScore<lfltScore)
    _maxScore = lfltScore;/*
  Serial.print("[_minScore:");
  Serial.print(_minScore);
  Serial.print("]");

  Serial.print("[_maxScore:");
  Serial.print(_maxScore);
  Serial.print("]");*/
  if ((_maxScore*0.8)>_minScore)
    return ((lfltScore-_minScore)/(_maxScore-_minScore));
  else
    return lfltScore;
}

float TempUnit::getRawScore(float fltVector[]){
  int lTmpSize = getDendriteSize();
  float lfltScore = 0, lfltTempValue=0;
  for (int i=0;i<lTmpSize;i++){
    lfltTempValue = _rawScore(fltVector[i],_dvalues[i],_std[i]);
    if (isnan(lfltTempValue))
      lfltTempValue = 0;
    lfltScore +=  lfltTempValue;
  }
  return lfltScore;
}

void TempUnit::showIndividualScore(float fltVector[]){
  int lTmpSize = getDendriteSize();
  float lfltScore = 0, lfltTempValue=0;
  for (int i=0;i<lTmpSize;i++){
    lfltTempValue = unitScore(fltVector[i],_dvalues[i],_weights[i],_std[i]);
    if (isnan(lfltTempValue))
      lfltTempValue = 0;
    lfltScore +=  lfltTempValue;
    Serial.print(i);
    Serial.print(": ");
    Serial.println(lfltTempValue);
  }
  Serial.print("TempUnit score : ");
  Serial.println(lfltScore);
}

void TempUnit::showDValues(){
  int lTmpSize = getDendriteSize();
  Serial.println("Mean Centered Values");
  for (int i=0;i<lTmpSize;i++){
    //Serial.print(i);
    //Serial.print(": ");
    Serial.println(_dvalues[i]);
  }
}

void TempUnit::showWeights(){
  int lTmpSize = getDendriteSize();
  Serial.println("Weights");
  for (int i=0;i<lTmpSize;i++){
  //  Serial.print(i);
  //  Serial.print(": ");
    Serial.println(_weights[i]);
  }
}

void TempUnit::showStd(){
  int lTmpSize = getDendriteSize();
  Serial.println("std");
  for (int i=0;i<lTmpSize;i++){
  //  Serial.print(i);
  //  Serial.print(": ");
    Serial.println(_std[i]);
  }
}

float TempUnit::unitScore(float lfltInput, float lfltDendrite, float lfltWeight,
  float lfltSigma){
    if (lfltSigma)
      return lfltWeight*_rawScore(lfltInput,lfltDendrite,lfltSigma);
    else
      return 0;
}

float TempUnit::_rawScore(float lfltInput, float lfltDendrite,float lfltSigma){
  return exp(-1*pow((lfltInput-lfltDendrite),2)/(lfltSigma));
}

int TempUnit::setDendriteSize(unsigned char lintSize){
  if (lintSize<MAXSIZE)
    _currentDendriteSize = lintSize;
  else
    _currentDendriteSize = MAXSIZE;
  for (int i=0;i<_currentDendriteSize;i++)
    _weights[i] /= _currentDendriteSize;
  return _currentDendriteSize;
}

unsigned char TempUnit::getDendriteSize(){
  return _currentDendriteSize;
}

void TempUnit::showDendriteLength(){
  Serial.println(getDendriteSize());
}

int getMaxDSize(){
  return MAXSIZE;
}
