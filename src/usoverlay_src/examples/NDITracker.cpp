/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*

  Author(s):  Ali Uneri
  Created on: 2009-10-13

  (C) Copyright 2009-2012 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

/*!
  \file
  \brief An example interface for NDI trackers with serial interface.
  \ingroup devicesTutorial
*/

#include <cisstCommon/cmnPath.h>
#include <cisstCommon/cmnUnits.h>
#include <cisstCommon/cmnGetChar.h>
#include <cisstOSAbstraction/osaThreadedLogFile.h>
#include <cisstMultiTask/mtsCollectorState.h>
#include <cisstMultiTask/mtsTaskManager.h>
#include <sawNDITracker/mtsNDISerial.h>
#include <sawNDITracker/mtsNDISerialControllerQtComponent.h>
#include <sawNDITracker/mtsNDISerialToolQtComponent.h>
#include <sawOpenIGTLink-dev/mtsOpenIGTLink.h>
#include <cisstCommon/cmnCommandLineOptions.h>
#include <string>

#include <QApplication>
#include <QMainWindow>

#include "polarisTCPServerTask.h" 

int main(int argc, char * argv[])
{
    // log configuration
    cmnLogger::SetMask(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskFunction(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskDefaultLog(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskClassMatching("mtsNDISerial", CMN_LOG_ALLOW_ALL);
    cmnLogger::AddChannel(std::cerr, CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);

    // create a Qt user interface
    QApplication application(argc,argv);
    // configure commandline options
    cmnCommandLineOptions options;

    // port number for oigtl
    std::string port;
    //std::string host;
    //int fps;
    
    // port number of clarity
    unsigned short cport; 
    
    options.AddOptionOneValue("op","oigtlport","port number for openigtlink server",cmnCommandLineOptions::REQUIRED_OPTION, &port);
    options.AddOptionOneValue("cp","clarityport","port number for polaris server",cmnCommandLineOptions::REQUIRED_OPTION, &cport);
    //options.AddOptionOneValue("f","fps","frame rate",cmnCommandlineOptions::REQUIRED_OPTION, &fps);

    std::string errorMessage;
    if (!options.Parse(argc, argv, errorMessage)) {
        std::cerr << "Error: " << errorMessage << std::endl;
        options.PrintUsage(std::cerr);
        return -1;
    }
    
    // create the components
    polarisTCPServerTask* polarisTCPServerTaskobj = new polarisTCPServerTask("ClarityServerTask", cport);
    polarisTCPServerTaskobj->Configure();
    mtsOpenIGTLink* myIGTLinkObj = new mtsOpenIGTLink("MyOpenIGTLink", 50.0 * cmn_ms);
    myIGTLinkObj->Configure(port);
    
    // add the components to the component manager
    mtsManagerLocal* componentManager = mtsComponentManager::GetInstance();
    componentManager->AddComponent(polarisTCPServerTaskobj);
    componentManager->AddComponent(myIGTLinkObj);

    if(!componentManager->Connect(myIGTLinkObj->GetName(), "requiresClarity", polarisTCPServerTaskobj->GetName(), "ProvidesClarityData")){
        CMN_LOG_INIT_ERROR << "Fail to connect mtsIGT to Clarity" << std::endl;
    }

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
    componentManager->KillAllAndWait(5.0 * cmn_s);
    componentManager->Cleanup();
    
    return 0;
}

