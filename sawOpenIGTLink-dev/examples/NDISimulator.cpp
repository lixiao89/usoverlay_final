#include <cisstCommon/cmnStrings.h>
#include <cisstCommon/cmnXMLPath.h>
#include <cisstVector/vctDynamicMatrixTypes.h>
#include <cisstOSAbstraction/osaSleep.h>
#include <cisstMultiTask/mtsInterfaceProvided.h>
#include <cmath>

#include "NDISimulator.h"

void NDISimulator::Construct(void){

  mtsInterfaceProvided* provided = AddInterfaceProvided("providesPositionCartesian");

  StateTable.AddData(MarkerPosition,"MarkerPosition");
  
  if(provided){
    provided->AddCommandReadState(StateTable, MarkerPosition, "GetPositionCartesian");
  }

  theta = 0;
}

void NDISimulator::Run(void){

  ProcessQueuedCommands();

  // generate a random tranformation matrix for testing (rotation around z)
  
  MarkerPosition.Position().Translation().X() = 0;
  MarkerPosition.Position().Translation().Y() = 0;
  MarkerPosition.Position().Translation().Z() = 0;
 
  MarkerPosition.Position().Rotation().Element(0,0) = cos(theta);
  MarkerPosition.Position().Rotation().Element(0,1) = -sin(theta);
  MarkerPosition.Position().Rotation().Element(0,2) = 0;
  
  MarkerPosition.Position().Rotation().Element(1,0) = sin(theta);
  MarkerPosition.Position().Rotation().Element(1,1) = cos(theta);
  MarkerPosition.Position().Rotation().Element(1,2) = 0;
      

  MarkerPosition.Position().Rotation().Element(2,0) = 0;
  MarkerPosition.Position().Rotation().Element(2,1) = 0;
  MarkerPosition.Position().Rotation().Element(2,2) = 1;

  theta += 0.1;
}
