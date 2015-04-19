#include <cisstCommon/cmnPath.h> 
#include <cisstCommon/cmnUnits.h>
#include <cisstMultiTask/mtsTaskManager.h>
#include <sawOpenIGTLink-dev/mtsOpenIGTLink.h>
#include <cisstOSAbstraction/osaSleep.h>
#include <cisstCommon/cmnGetChar.h>
#include <cisstCommon/cmnCommandLineOptions.h>

#include "NDISimulator.h"      

int main(int argc, char* argv[]){

  // configure commandline options
  cmnCommandLineOptions options;
  std::string port;
  //std::string host;
  //int fps;

  options.AddOptionOneValue("p","port","port number",cmnCommandLineOptions::REQUIRED_OPTION, &port);
  //options.AddOptionOneValue("f","fps","frame rate",cmnCommandlineOptions::REQUIRED_OPTION, &fps);

  std::string errorMessage;
  if (!options.Parse(argc, argv, errorMessage)) {
    std::cerr << "Error: " << errorMessage << std::endl;
    options.PrintUsage(std::cerr);
    return -1;
  }

  mtsComponentManager * componentManager = mtsComponentManager::GetInstance();
  
  //create components
  mtsOpenIGTLink* myIGTLinkObj = new mtsOpenIGTLink("MyOpenIGTLink", 50.0 * cmn_ms); 
  myIGTLinkObj->Configure(port);
  NDISimulator* myNDISimulator = new NDISimulator("MyNDISimulator", 50.0 * cmn_ms);

  //add components
  componentManager->AddComponent(myIGTLinkObj);
  componentManager->AddComponent(myNDISimulator);

  // connect componenents
  componentManager->Connect(myIGTLinkObj->GetName(),"requiresPositionCartesian",
			    myNDISimulator->GetName(),"providesPositionCartesian");

  // create and start all components
  componentManager->CreateAll();
  componentManager->WaitForStateAll(mtsComponentState::READY);
  componentManager->StartAll();
  componentManager->WaitForStateAll(mtsComponentState::ACTIVE);

  char c = cmnGetChar();
  
  // loop until GUI exits
  while (c != 'q') {
    osaSleep(10.0 * cmn_ms);
  }
  
  // kill all components and perform cleanup
  componentManager->KillAll();
  componentManager->WaitForStateAll(mtsComponentState::FINISHED, 2.0 * cmn_s);
  componentManager->Cleanup();

  return 0;
}
