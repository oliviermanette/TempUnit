#include "tunet.h"

TUNet::TUNet(unsigned char NbNeurons){
  _GuchrTUNetSize = NbNeurons;
}

unsigned char TUNet::getTUNetSize(){
  return _GuchrTUNetSize;
}

unsigned char TUNet::setNewTU(float fltVector[]){
  if (_GuchrTUNetSize<MAXNETSIZE){
    Network[_GuchrTUNetSize].setNewTU(fltVector);
    Serial.print("New TempUnit neuron added to the network with the #ID:");
    Serial.println(_GuchrTUNetSize);
    Network[_GuchrTUNetSize].setPoolID(_GuchrTUNetSize);
    _GuchrTUNetSize++;
    return _GuchrTUNetSize;
  }
  else
    return 0;
}

unsigned char TUNet::getPoolsNumber(){
  unsigned char uchrResult=0;
  for (unsigned char i=0;i<_GuchrTUNetSize;i++){
    if (Network[i].getPoolID()>uchrResult)
      uchrResult = Network[i].getPoolID();
  }
  return uchrResult+1;
}

unsigned char TUNet::getPoolSize(unsigned char uchrPoolID){
  unsigned char uchrResult=0;
  for (unsigned char i=0;i<_GuchrTUNetSize;i++){
    if (Network[i].getPoolID()==uchrPoolID)
      uchrResult++;
  }
  return uchrResult;
}

unsigned char TUNet::getIDofMaxSPool(unsigned char uchrPoolID, float fltVector[]){
  unsigned char uchrResult=0;
  float lflTmpScore=0;
  for (unsigned char i=0;i<_GuchrTUNetSize;i++){
    if (Network[i].getPoolID()==uchrPoolID){
      if (Network[i].getScore(fltVector)>lflTmpScore){
        lflTmpScore = Network[i].getScore(fltVector);
        uchrResult = i;
      }
    }
  }
  return uchrResult;
}

int TUNet::learnNewVector(unsigned char TUId, float fltVector[], int lintReinforcement){
  unsigned char lchSuccess = setNewTU(fltVector);
  int lintSize = Network[TUId].getDendriteSize();
  for (int i=0;i<lintSize;i++)
      IIN(i,fltVector[i]);
  for (int i=0;i<_GuchrTUNetSize;i++)
      Network[i].normalizeAllWeights();

  if (lchSuccess){
      Network[lchSuccess-1].setPoolID(TUId);
      return 0;
  }
  TUId = getIDofMaxSPool(TUId, fltVector);

  return Network[TUId].learnNewVector(fltVector,lintReinforcement);
}

void TUNet::IIN(int lIntPos, float lfltInputValue){
  int lintNbNeurons=0;
  for (unsigned char i=0;i<_GuchrTUNetSize;i++)
    if (Network[i].isSynapse(lIntPos, lfltInputValue)>MINSCORE)
      lintNbNeurons++;
  //Serial.print("nb synapses identiques : ");
  //Serial.println(lintNbNeurons);
  if (lintNbNeurons==_GuchrTUNetSize){
    //Serial.print(lintNbNeurons);
    //Serial.println(" : unspecific Synapse Detected");
    for (unsigned char i=0;i<_GuchrTUNetSize;i++)
      Network[i].setDecWeightRatio(lIntPos, 0);
  }
}

int TUNet::setDendriteSize(unsigned char TUId, int lintSize){
  return Network[TUId].setDendriteSize(lintSize);
}

void TUNet::setAllNetworkDendriteSize(int lintSize){
  for (unsigned char i=0;i<MAXNETSIZE;i++)
    Network[i].setDendriteSize(lintSize);
}

void TUNet::setPoolID (unsigned char TUId, char lchrIdent){
  Network[TUId].setPoolID(lchrIdent);
}

char TUNet::getPoolID (unsigned char TUId){
  return Network[TUId].getPoolID();
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

void TUNet::showAllPoolScore(float fltVector[]){
  for (unsigned char i=0; i<_GuchrTUNetSize;i++){
    Serial.print(i);
    Serial.print(": ");
    Serial.println(Network[i].getScore(fltVector));
  }
}

void TUNet::showAllPoolParameters(){
  for (unsigned char i=0; i<_GuchrTUNetSize;i++){
    Network[i].showDValues();
    Network[i].showWeights();
    Network[i].showStd();
  }
}

unsigned char TUNet::getWinnerID(float fltVector[], bool verbose){
  unsigned char lchrMaxID=0;
  float currentOutput, lfltMaxOutput = 0;

  for (unsigned char i=0; i<_GuchrTUNetSize;i++){
    currentOutput = Network[i].getScore(fltVector);
    if (verbose){
      Serial.print(i);
      Serial.print(": ");
      Serial.println(currentOutput);
    }
    if (currentOutput>lfltMaxOutput){
      lfltMaxOutput = currentOutput;
      lchrMaxID = Network[i].getPoolID();
    }
  }
  return lchrMaxID;
}
