#include "tunet.h"

TUNet::TUNet(unsigned char NbNeurons){
  _GuchrTUNetSize = NbNeurons;
}

unsigned char TUNet::getTUNetSize(){
  return _GuchrTUNetSize;
}

bool TUNet::setNewTU(float fltVector[]){
  if (_GuchrTUNetSize<MAXNETSIZE){
    Network[_GuchrTUNetSize].setNewTU(fltVector);
    _GuchrTUNetSize++;
    Serial.print("New TempUnit neuron added to the network with the #ID:");
    Serial.println(_GuchrTUNetSize);
    return true;
  }
  else
    return false;
}

int TUNet::learnNewVector(unsigned char TUId, float fltVector[], int lintReinforcement){
  return Network[TUId].learnNewVector(fltVector,lintReinforcement);
}

int TUNet::setDendriteSize(unsigned char TUId, int lintSize){
  return Network[TUId].setDendriteSize(lintSize);
}

void TUNet::setAllNetworkDendriteSize(int lintSize){
  for (unsigned char i=0;i<MAXNETSIZE;i++)
    Network[i].setDendriteSize(lintSize);
}

void TUNet::setIdentification (unsigned char TUId, char lchrIdent){
  Network[TUId].setIdentification(lchrIdent);
}

char TUNet::getIdentification (unsigned char TUId){
  return Network[TUId].getIdentification();
}

float TUNet::getScore(unsigned char TUId, float fltVector[]){
  return Network[TUId].getScore(fltVector);
}

int TUNet::getDendriteSize(unsigned char TUId){
  return Network[TUId].getDendriteSize();
}

int TUNet::getMaxDSize(unsigned char TUId){
  return Network[TUId].getMaxDSize();
}

void TUNet::showIndividualScore(unsigned char TUId,float fltVector[]){
  Network[TUId].showIndividualScore(fltVector);
}

void TUNet::showDValues(unsigned char TUId){
  Network[TUId].showDValues();
}

void TUNet::showWeights(unsigned char TUId){
  Network[TUId].showWeights();
}

void TUNet::showStd(unsigned char TUId){
  Network[TUId].showStd();
}

unsigned char TUNet::getWinnerID(float fltVector[]){
  unsigned char lchrMaxID=0;
  float currentOutput, lfltMaxOutput = 0;

  for (unsigned char i=0; i<_GuchrTUNetSize;i++){
    currentOutput = Network[i].getScore(fltVector);
    Serial.print(i);
    Serial.print(": ");
    Serial.println(currentOutput);
    if (currentOutput>lfltMaxOutput){
      lfltMaxOutput = currentOutput;
      lchrMaxID = i;
    }
  }
  return lchrMaxID;
}
