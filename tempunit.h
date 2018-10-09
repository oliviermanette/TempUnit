/*
TempUnit.h Library to make a TempUnit Artificial Neural Network
on a Arduino Compatible device
each Arduino will work as a bunch of neurons that can interplay with others
Arduino boards.
Created by : F.L. OLivier David MANETTE on October 2018
*/

#ifndef Tempunit_h
#define Tempunit_h

#define MAXSIZE 5

#include "Arduino.h"

class TempUnit{
  public:
    TempUnit();
    void setNewTU(float *):
    int learnNewVector(float *, int lintReinforcement=1);
    int setDendriteSize(int lintSize);
    float getScore(float *);
    int getDendriteSize();
    int getMaxDSize();

  private:
    float unitScore(float lfltInput, float lfltDendrite, float lfltWeight,
      float lfltSigma);
    float sum(float *lfltValues);
    const float _pi = 3.14159;
    float _dvalues[MAXSIZE];
    float _weights[MAXSIZE];
    float _std[MAXSIZE];
    int _nb; //nombre de fois que c'est updaté
    int _currentDendriteSize;
    float _k[MAXSIZE]; //coefficients pour la moyenne mobile

};

#endif
