//#include "Arduino.h"
#include "tempunit.h"

TempUnit::TempUnit(){
  _currentDendriteSize = MAXSIZE;
  _identity = -1;

  for (int i=0;i<MAXSIZE;i++){
    _dvalues[i]=0;
    _weights[i]=1;
    _std[i]=1;
    _k[i]=0;
  }
  _nb = 0;

}

void TempUnit::setIdentification (char lchrIdent){
  if (_identity==-1)
    _identity = lchrIdent;
}

char TempUnit::getIdentification (){
  return _identity;
}

void TempUnit::setNewTU(float fltVector[]){
  //Add all values to _dvalues
  int lTmpSize = getDendriteSize();

  Serial.println("New TempUnit neuron added! ");

  for (int i=0;i<lTmpSize;i++){
    _dvalues[i]= fltVector[i];
  }
  Serial.println("°_°");
}

int TempUnit::learnNewVector(float fltVector[], int lintReinforcement){
  int lTmpSize = getDendriteSize();
  float lfltTempScore, lfltTempNewK, lfltTmpKp;
  _nb++;

  for (int i=0;i<lTmpSize;i++){
    lfltTempScore = _rawScore(fltVector[i],_dvalues[i],_std[i]);
    _activationsDendritique[i]=lfltTempScore;
    lfltTempNewK = lintReinforcement * lfltTempScore/_nb;
    _weights[i] += lfltTempNewK;
    if (_weights[i]<0)
      _weights[i]=0;
    lfltTmpKp = _k[i]+lfltTempNewK;
    if ((lintReinforcement>0)&&(lfltTmpKp))
        _dvalues[i] = (_k[i]*_dvalues[i]/lfltTmpKp)
        + (lfltTempNewK/lfltTmpKp)*fltVector[i];
    _k[i]+=lfltTempNewK;
    _std[i]+= lintReinforcement * (0.5 - lfltTempScore)/_nb;
  }
  normalizeAllWeights();
  return _nb;
}

void TempUnit::normalizeAllWeights(){
  int lTmpSize = getDendriteSize();
  float lfltSumWeight = sum(&_weights[0]);
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

bool TempUnit::isSynapse(int lIntPos, float lfltMean, float lfltStd){
  if((_dvalues[lIntPos]==lfltMean)/*&&(_std[lIntPos]==lfltStd)*/)
    return true;
  else
    return false;
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
    Serial.print(i);
    Serial.print(": ");
    Serial.println(_dvalues[i]);
  }
}

void TempUnit::showWeights(){
  int lTmpSize = getDendriteSize();
  Serial.println("Weights");
  for (int i=0;i<lTmpSize;i++){
    Serial.print(i);
    Serial.print(": ");
    Serial.println(_weights[i]);
  }
}

void TempUnit::showStd(){
  int lTmpSize = getDendriteSize();
  Serial.println("std");
  for (int i=0;i<lTmpSize;i++){
    Serial.print(i);
    Serial.print(": ");
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

int TempUnit::setDendriteSize(int lintSize){
  if (lintSize<MAXSIZE)
    _currentDendriteSize = lintSize;
  else
    _currentDendriteSize = MAXSIZE;
  for (int i=0;i<_currentDendriteSize;i++)
    _weights[i] /= _currentDendriteSize;
  return _currentDendriteSize;
}

int TempUnit::getDendriteSize(){
  return _currentDendriteSize;
}

int getMaxDSize(){
  return MAXSIZE;
}
