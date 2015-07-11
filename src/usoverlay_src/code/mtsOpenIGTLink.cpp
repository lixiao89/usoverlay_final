/* *- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*


  Author(s):  Ali Uneri
  Created on: 2009-08-10

  (C) Copyright 2009-2012 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstOSAbstraction/osaSleep.h>
#include <cisstMultiTask/mtsInterfaceProvided.h>
#include <cisstMultiTask/mtsInterfaceRequired.h>
#include <sawOpenIGTLink-dev/mtsOpenIGTLink.h>

CMN_IMPLEMENT_SERVICES_DERIVED_ONEARG(mtsOpenIGTLink, mtsTaskPeriodic, mtsTaskPeriodicConstructorArg);

void mtsOpenIGTLink::Cleanup(void)
{
    CMN_LOG_CLASS_INIT_VERBOSE << "~mtsOpenIGTLink: closing hanging connections" << std::endl;
    if (serverSocket) serverSocket->CloseSocket();
}


void mtsOpenIGTLink::Configure(const std::string & hostAndPort)
{
    // // parse port and [if any] host information
    std::string host;
    unsigned int port;
    size_t colonPosition = hostAndPort.find(':');
    if (colonPosition == std::string::npos) {
        host = "";
        port = atoi(hostAndPort.c_str());
    } else {
        host = hostAndPort.substr(0, colonPosition);
        port = atoi(hostAndPort.substr(colonPosition + 1).c_str());
    }

    // if a target host is not provided, configure as server, otherwise client
    if (host.empty()) {
        //CMN_LOG_CLASS_INIT_VERBOSE << "mtsOpenIGTLink: setting up as server" << std::endl;
        std::cout << "mtsOpenIGTLink: setting up as server in port: " << port << std::endl; 
        ConnectionType = SERVER;
        serverSocket = igtl::ServerSocket::New();
        ServerPort = port;
    } else {
        CMN_LOG_CLASS_INIT_VERBOSE << "mtsOpenIGTLink: setting up as client" << std::endl;
        // ConnectionType = CLIENT;
        // Host = host;
        // Port = port;
        // SocketServer = 0;
        // Socket = new osaSocket();
        // IGTLData = new sawOpenIGTLinkData;
    }
    IsConnected = false;

    /* The required interface that (this interafce grab data directly from sawNDITracker):
       1. grabs data from a provided interface (sawNDITracker for example)
       2. publishes data in form of openigtlink */
    // mtsInterfaceRequired * requiresPositionCartesion = AddInterfaceRequired("RobotBase", MTS_OPTIONAL);
    // if (requiresPositionCartesion){
    //     // get tool cartesian pose from mtsNDITracker
    //     requiresPositionCartesion->AddFunction("GetPositionCartesian", NDI.GetPositionCartesian);
    // }

    // required interface to grab data from polarisTCPServer component (this interafce grab data from a serial port provided by clarity_sim or real clarity)
    mtsInterfaceRequired* requiresClarityInterface = AddInterfaceRequired("requiresClarity", MTS_OPTIONAL);
    if(requiresClarityInterface){
      requiresClarityInterface->AddFunction("GetClarityRobotBase", GetClarityRobotBase);
      //requiresClarityInterface->AddFunction("CloseConnection", CloseConnection);
    }

}


void mtsOpenIGTLink::Startup(void)
{
    if (ConnectionType == SERVER) {
        int r = serverSocket->CreateServer(ServerPort);
        if (r<0){
            CMN_LOG_CLASS_INIT_VERBOSE << "Cannot create a server socket" << std::endl;
            exit(0);
        }
    }
}


void mtsOpenIGTLink::Run(void)
{
    ProcessQueuedCommands();

    if (!IsConnected) {
        if (ConnectionType == SERVER) {
            do {
                socket = serverSocket->WaitForConnection(1000);
            } while (!socket.IsNotNull());
            IsConnected = true;
        }//  else if (ConnectionType == CLIENT) {
        //     IsConnected = Socket->Connect(Host.c_str(), Port);
        // }
    }

    // populate NDI.PositionCartesian according to the type of interface connected
    //   NDI.GetPositionCartesian(NDI.PositionCartesian);


    vct4x4 temp;
    GetClarityRobotBase(temp);
    vctFrm4x4 tip_frame;
    tip_frame.Assign(temp);
    //std::cout<<tip_frame<<std::endl;
    
    if (IsConnected) {
        igtl::TransformMessage::Pointer transMsg;
        transMsg = igtl::TransformMessage::New();
        transMsg->SetDeviceName("OpenIGTLink_USProbe");

        igtl::Matrix4x4 dataMatrix;
        prmPositionCartesianToOIGTL(tip_frame, dataMatrix);
        transMsg->SetMatrix(dataMatrix);
        transMsg->Pack();
        socket->Send(transMsg->GetPackPointer(), transMsg->GetPackSize());

        igtl::PrintMatrix(dataMatrix);
    }
}


void mtsOpenIGTLink::prmPositionCartesianToOIGTL(const vctFrm4x4& frameCISST,
                                                 igtl::Matrix4x4& frameIGTL){

    frameIGTL[0][0] = frameCISST.Rotation().Element(0,0);
    frameIGTL[1][0] = frameCISST.Rotation().Element(1,0);
    frameIGTL[2][0] = frameCISST.Rotation().Element(2,0);
    frameIGTL[3][0] = 0;

    frameIGTL[0][1] = frameCISST.Rotation().Element(0,1);
    frameIGTL[1][1] = frameCISST.Rotation().Element(1,1);
    frameIGTL[2][1] = frameCISST.Rotation().Element(2,1);
    frameIGTL[3][1] = 0;

    frameIGTL[0][2] = frameCISST.Rotation().Element(0,2);
    frameIGTL[1][2] = frameCISST.Rotation().Element(1,2);
    frameIGTL[2][2] = frameCISST.Rotation().Element(2,2);
    frameIGTL[3][2] = 0;

    frameIGTL[0][3] = frameCISST.Translation().Element(0) - 250;
    frameIGTL[1][3] = frameCISST.Translation().Element(1) - 42.3;
    frameIGTL[2][3] = frameCISST.Translation().Element(2) + 1592;
    frameIGTL[3][3] = 1;

}
