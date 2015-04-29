#include <cisstCommon/cmnPath.h>
#include <cisstCommon/cmnUnits.h>
#include <cisstOSAbstraction/osaThreadedLogFile.h>
#include <cisstMultiTask/mtsCollectorState.h>
#include <cisstMultiTask/mtsTaskManager.h>
#include <sawNDITracker/mtsNDISerial.h>
#include <cisstCommon/cmnCommandLineOptions.h>
#include "guiTask.h"
#include "clarityClientTask.h"
#include <iostream>
#include "polarisToolComponent.h"


int main(int argc, char *argv[])
{
    // log configuration
    cmnLogger::SetMask(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskFunction(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskDefaultLog(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskClassMatching("mtsNDISerial", CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);
    cmnLogger::AddChannel(std::cerr, CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);


    // add commandline options to input port number
    cmnCommandLineOptions options;

    unsigned short port;

    options.AddOptionOneValue("cp", "clarityport","port number for polaris", cmnCommandLineOptions::REQUIRED_OPTION, &port);

    std::string errorMessage;                                                               

    if (!options.Parse(argc, argv, errorMessage)) {                                        
      std::cerr << "Error: " << errorMessage << std::endl;                                  
      options.PrintUsage(std::cerr);                                                        
      return -1;                                                                            
    }    

    // create the components
    mtsNDISerial * componentNDISerial = new mtsNDISerial("componentNDISerial", 50.0 * cmn_ms); 
    guiTask* guiTaskObj = new guiTask("clarityGUI");
    clarityClientTask * clarityClientTaskObj = new clarityClientTask("ClarityTask", 50.0*cmn_ms, port);
  
    clarityClientTaskObj->Configure();
  
    std::cout<<"here3"<<std::endl;  
    // configure the components
    cmnPath searchPath;
    searchPath.Add(cmnPath::GetWorkingDirectory());
    componentNDISerial->Configure(searchPath.Find("config.xml"));
    std::cout<<"2"<<std::endl;
    // add the components to the component manager
    mtsTaskManager * taskManager = mtsTaskManager::GetInstance();
	
    taskManager->AddComponent(guiTaskObj);
    taskManager->AddComponent(componentNDISerial);
    taskManager->AddComponent(clarityClientTaskObj);

    // connect the components, e.g. RequiredInterface -> ProvidedInterface
    taskManager->Connect(guiTaskObj->GetName(), "guiRequiresNDI", componentNDISerial->GetName(), "Controller");
    taskManager->Connect(clarityClientTaskObj->GetName(), "clarityRequiresGUI",guiTaskObj->GetName(), "guiProvidesClarity");
    
    // add interfaces for tools.. First one must be Robot Base and second should be Tip
    for (unsigned int i = 0; i < componentNDISerial->GetNumberOfTools(); i++) {
        std::string toolName = componentNDISerial->GetToolName(i);
        polarisToolComponent * componentToolComponent = new polarisToolComponent(toolName,toolName);
        taskManager->AddComponent(componentToolComponent);
        taskManager->Connect(toolName, toolName,componentNDISerial->GetName(), toolName);
	if(i==0)
	  taskManager->Connect(guiTaskObj->GetName(), "guiRequiresRobotBase",toolName, toolName);
	else
	  taskManager->Connect(guiTaskObj->GetName(), "guiRequiresRobotTip",toolName, toolName);
    }

    // create and start all components
    taskManager->CreateAll();
    taskManager->WaitForStateAll(mtsComponentState::READY, 2.0 * cmn_s);
    taskManager->StartAll();
    taskManager->WaitForStateAll(mtsComponentState::ACTIVE, 2.0 * cmn_s);

	while (!(guiTaskObj->IsTerminated())) {
        osaSleep(0.01);     
    }
}
