/*
TempUnit.h Library to make a TempUnit Artificial Neural Network
on a Arduino Compatible device
each Arduino will work as a bunch of neurons that can interplay with others
Arduino boards.
Created by : F.L. OLivier David MANETTE on October 2018
*/

#ifndef Tempunit_h
#define Tempunit_h

#define MAXSIZE 64

#include "Arduino.h"

class TempUnit{
  public:
    TempUnit();
    void setNewTU(float fltVector[]);
    int learnNewVector(float fltVector[], int lintReinforcement=1);
    int setDendriteSize(int lintSize);
    void setIdentification (char);
    char getIdentification ();
    float getScore(float fltVector[]);
    int getDendriteSize();
    int getMaxDSize();
    void showIndividualScore(float fltVector[]);
    void showDValues();
    void showWeights();
    void setDecWeightRatio(int lintIndex, float fltRatio);
    void showStd();
    float getActivation(int lintDendritePosition);
    void normalizeAllWeights(bool lblIsAbove1=false);
    float getSynapseMean(int lIntPos);
    float getSynapseStd(int lIntPos);
    float getSynapseWeight(int lIntPos);
    float isSynapse(int lIntPos, float lfltInputValue);

  private:
    float unitScore(float lfltInput, float lfltDendrite, float lfltWeight,
      float lfltSigma);

    float _rawScore(float lfltInput, float lfltDendrite,float lfltSigma);
    float sum(float *lfltValues);
    const float _pi = 3.14159;
    float _dvalues[MAXSIZE]; // centre de réponse
    float _weights[MAXSIZE]; // poids synaptique
    float _std[MAXSIZE]; // écart-type
    float _activationsDendritique[MAXSIZE];//Les apprentissages dépendent de ça
    char _identity;
    int _nb; //nombre de fois que c'est updaté
    int _currentDendriteSize;
    float _k[MAXSIZE]; //coefficients pour la moyenne mobile

};

#endif
