/*
TUNet.h Library to make a TempUnit Artificial Neural Network
on a Arduino Compatible device
each Arduino board will work as a bunch of neurons that can interplay with others
Arduino boards.
This class create a network of max 64 neurons
It manages the connexions and transmission of signal between neurons if there are
Created by : F.L. OLivier David MANETTE on October 2018
*/

#ifndef TUNet_h
#define TUNet_h
#include "Arduino.h"

#define MAXNETSIZE 64

#define MINSCORE 0.999
#define DECRATIO 1

#include <SD.h>
#include <SPI.h>

#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13

#include "tempunit.h"

class TUNet{
  public:
    TUNet(unsigned char NbNeurons=0);
    unsigned char getTUNetSize();
    unsigned char getPoolSize(unsigned char uchrPoolID);
    void showPoolSize(unsigned char uchrPoolID);
    unsigned char getPoolsNumber();
    void showPoolsNumber();
    unsigned char getIDofMaxSPool(unsigned char uchrPoolID, float fltVector[]);
    float getMaxScoreOfPool(unsigned char uchrPoolID, float fltVector[]);
    void showMaxOfPoolScore(float fltVector[]);
    bool setMaxNetSize(unsigned char uchrMaxNetSize); // to limit pool size during learning
    bool setNetSize(unsigned char uchrNetSize);
    bool selectNeuron(unsigned char luchrNeuron);
    unsigned char getSelectedNeuron();
    void showSelectedNeuron();
    bool selectSynapse(unsigned char luchrNeuron);
    unsigned char getSelectedSynapse();
    void showSelectedSynapse();
    bool setWeight(float lfltValue);
    bool setDValue(float lfltValue);
    bool setStd(float lfltValue);
    float getWeight();
    float getDValue();
    float getStd();
    void setPoolID(unsigned char lchrPoolID);

    //Wrapper for TempUnit functions :
    unsigned char setNewTU(float fltVector[]);
    int learnNewVector(unsigned char TUId, float fltVector[], int lintReinforcement=1);
    int setDendriteSize(unsigned char TUId, int lintSize);
    int setDendriteSize(int lintSize);
    void showDendriteLength(unsigned char TUId);
    void setAllNetworkDendriteSize(int lintSize);
    void setPoolID (unsigned char TUId, unsigned char);
    unsigned char getPoolID (unsigned char TUId);
    void showPoolID(unsigned char TUId);

    void setNetID(unsigned char);
    unsigned char getNetID();
    void showNetID();

    float getScore(unsigned char TUId, float fltVector[]);
    unsigned char getDendriteSize(unsigned char TUId);
    int getMaxDSize(unsigned char TUId);
    unsigned char getWinnerID(float fltVector[], bool verbose=false);
    void showAllPoolScore(float fltVector[]);

    void showAllPoolParameters();
    void showIndividualScore(unsigned char TUId,float fltVector[]);
    void showDValues(unsigned char TUId);
    void showWeights(unsigned char TUId);
    void showStd(unsigned char TUId);

    //bool saveEvent(float fltVector[], unsigned char TUId, unsigned char luchrDendriteLength);

    void setNetTimeShift(unsigned long);
    unsigned long getNetTimeStamp();
    void showNetTimeStamp();

  private:
    TempUnit Network[MAXNETSIZE];
    unsigned char _GuchrTUNetSize;
    unsigned char _GuchrMaxNetSize;
    unsigned char _SelectedNeuron;
    unsigned char _SelectedSynapse;
    unsigned char _NetID;
    unsigned long _TimeShift;

    void IIN(int lIntPos, float lfltInputValue); // inhibitory interneuron
};

#endif
