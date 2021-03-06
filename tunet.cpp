#include "tunet.h"

TUNet::TUNet(unsigned char NbNeurons){
  _GuchrMaxNetSize = MAXNETSIZE;
  _NetID = 0;
  _TimeShift = 0;
  if (NbNeurons<_GuchrMaxNetSize)
    _GuchrTUNetSize = NbNeurons;
  else
    _GuchrTUNetSize = _GuchrMaxNetSize;
}

bool TUNet::selectNeuron(unsigned char luchrNeuron){
  if (luchrNeuron<_GuchrTUNetSize){
    _SelectedNeuron = luchrNeuron;
    return true;
  }
  return false;
}


unsigned char TUNet::getSelectedNeuron(){
  return _SelectedNeuron;
}

void TUNet::showSelectedNeuron(){
  Serial.println(_SelectedNeuron);
}

bool TUNet::setWeight(float lfltValue){
  return Network[_SelectedNeuron].setWeight(_SelectedSynapse, lfltValue);
}

bool TUNet::setDValue(float lfltValue){
  return Network[_SelectedNeuron].setDValue(_SelectedSynapse, lfltValue);
}

bool TUNet::setStd(float lfltValue){
  return Network[_SelectedNeuron].setStd(_SelectedSynapse, lfltValue);
}

float TUNet::getWeight(){
  return Network[_SelectedNeuron].getSynapseWeight(_SelectedSynapse);
}

float TUNet::getDValue(){
  return Network[_SelectedNeuron].getSynapseMean(_SelectedSynapse);
}

float TUNet::getStd(){
  return Network[_SelectedNeuron].getSynapseStd(_SelectedSynapse);
}

bool TUNet::selectSynapse(unsigned char luchrNeuron){
  if (luchrNeuron<Network[_SelectedNeuron].getDendriteSize()){
    _SelectedSynapse = luchrNeuron;
    return true;
  }
  return false;
}

unsigned char TUNet::getSelectedSynapse(){
  return _SelectedSynapse;
}

void TUNet::showSelectedSynapse(){
  Serial.println(_SelectedSynapse);
}

unsigned char TUNet::getTUNetSize(){
  return _GuchrTUNetSize;
}

bool TUNet::setMaxNetSize(unsigned char uchrMaxNetSize){
  if (_GuchrTUNetSize<uchrMaxNetSize){
    _GuchrMaxNetSize = uchrMaxNetSize;
    return true;
  }
  return false;
}

bool TUNet::setNetSize(unsigned char uchrNetSize){
  if (uchrNetSize<=_GuchrMaxNetSize){
    _GuchrTUNetSize = uchrNetSize;
    return true;
  }
  return false;
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

void TUNet::showPoolSize(unsigned char uchrPoolID){
  Serial.println(getPoolSize(uchrPoolID));
}

void TUNet::showPoolsNumber(){
  Serial.println(getPoolsNumber());
}

unsigned char TUNet::getIDofMaxSPool(unsigned char uchrPoolID, float fltVector[]){
  unsigned char uchrResult=0;
  float lflTmpScore=0;
  for (unsigned char i=0;i<_GuchrTUNetSize;i++){
    if (Network[i].getPoolID()==uchrPoolID){
      if (Network[i].getRawScore(fltVector)>lflTmpScore){
        lflTmpScore = Network[i].getRawScore(fltVector);
        uchrResult = i;
      }
    }
  }
  return uchrResult;
}

float TUNet::getMaxScoreOfPool(unsigned char uchrPoolID, float fltVector[]){
  float lflTmpScore=0;
  for (unsigned char i=0;i<_GuchrTUNetSize;i++)
    if (Network[i].getPoolID()==uchrPoolID)
      if (Network[i].getScore(fltVector)>lflTmpScore)
        lflTmpScore = Network[i].getScore(fltVector);
  return lflTmpScore;
}

void TUNet::showMaxOfPoolScore(float fltVector[]){
  unsigned char lchrNb = getPoolsNumber();
  for (unsigned char i=0;i<lchrNb;i++)
    Serial.println(getMaxScoreOfPool(i,fltVector));
}

int TUNet::learnNewVector(unsigned char TUId, float fltVector[], int lintReinforcement){
  //Create new TU only if _maxScore<0.8
  unsigned char lchSuccess = 0;
  if (getMaxScoreOfPool(TUId,fltVector)<0.8)
    lchSuccess = setNewTU(fltVector);

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

int TUNet::setDendriteSize(int lintSize){
  return Network[getSelectedNeuron()].setDendriteSize(lintSize);
}

void TUNet::setAllNetworkDendriteSize(int lintSize){
  for (unsigned char i=0;i<MAXNETSIZE;i++)
    Network[i].setDendriteSize(lintSize);
}

void TUNet::setPoolID (unsigned char TUId, unsigned char lchrIdent){
  Network[TUId].setPoolID(lchrIdent);
}

void TUNet::setPoolID(unsigned char lchrPoolID){
  Network[_SelectedNeuron].setPoolID(lchrPoolID);
}

unsigned char TUNet::getPoolID (unsigned char TUId){
  return Network[TUId].getPoolID();
}

void TUNet::setNetTimeShift(unsigned long luintTS){
  _TimeShift = luintTS;
}

unsigned long TUNet::getNetTimeStamp(){
  return round(millis()/1000) + _TimeShift;
}

void TUNet::showNetTimeStamp(){
  Serial.println(getNetTimeStamp());
}

void TUNet::showPoolID(unsigned char TUId){
  Serial.println(Network[TUId].getPoolID());
}

void TUNet::TUNet::setNetID(unsigned char luchrID){
  _NetID = luchrID;
}

unsigned char TUNet::getNetID(){
  return _NetID;
}

void TUNet::showNetID(){
  Serial.println(_NetID);
}

float TUNet::getScore(unsigned char TUId, float fltVector[]){
  return Network[TUId].getScore(fltVector);
}

unsigned char TUNet::getDendriteSize(unsigned char TUId){
  return Network[TUId].getDendriteSize();
}

void TUNet::showDendriteLength(unsigned char TUId){
  Network[TUId].showDendriteLength();
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
    Serial.print("(");
    Serial.print(Network[i].getPoolID());
    Serial.print(") : ");
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
  unsigned char lchrPoolID=0;//, lchrMaxID=0;
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
      lchrPoolID = Network[i].getPoolID();
      //lchrMaxID = i;
    }
  }
  //saveEvent(fltVector, lchrMaxID, Network[lchrMaxID].getDendriteSize());
  return lchrPoolID;
}
