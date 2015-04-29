#include "polarisTCPServerTask.h"

typedef vctFixedSizeVector <double, 13> vct13;
typedef mtsGenericObjectProxy<vct13> mtsVct13;
CMN_DECLARE_SERVICES_INSTANTIATION(mtsVct13);
CMN_IMPLEMENT_SERVICES_TEMPLATED(mtsVct13);

CMN_IMPLEMENT_SERVICES(polarisTCPServerTask);
polarisTCPServerTask::polarisTCPServerTask(const char *name, unsigned short cport) : mtsTaskContinuous(name){
  
  StateTable.AddData(F_RobotBase,"RobotBase");
  StateTable.AddData(F_RobotTip,"RobotTip");

  mtsInterfaceProvided *providedInterface;
  providedInterface = AddInterfaceProvided("ProvidesClarityData");
  providedInterface->AddCommandReadState(StateTable, F_RobotBase,"GetClarityRobotBase");
  providedInterface->AddCommandReadState(StateTable, F_RobotTip,"GetClarityRobotTip");
  // providedInterface->AddCommandVoid(&polarisTCPServerTask::CloseSocket,this,"CloseConnection");

  //port = 8089;
  port = cport;
  isServerConnected = false; 
  connectionEstablished = false;
  F_RobotBase.SetAll(0.0);
  F_RobotBase.Element(0,0) = 1.0;
  F_RobotBase.Element(1,1) = 1.0;
  F_RobotBase.Element(2,2) = 1.0;
  F_RobotBase.Element(3,3) = 1.0;

  F_RobotTip.SetAll(0.0);
  F_RobotTip.Element(0,0) = 1.0;
  F_RobotTip.Element(1,1) = 1.0;
  F_RobotTip.Element(2,2) = 1.0;
  F_RobotTip.Element(3,3) = 1.0;
  socket = 0;

}
void polarisTCPServerTask::Configure(const std::string & CMN_UNUSED(filename)){ 
  cmnLogger::SetMask(CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);
  cmnLogger::AddChannel(std::cerr, CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);
  cmnLogger::SetMaskClass("osaSocket", CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);
  cmnLogger::SetMaskClass("osaSocketServer", CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);

  while (!socketServer.AssignPort(port)) {
    std::cout << "Couldn't connect to the server yet" << std::endl;
    osaSleep(5.0 * cmn_s);
  }
  isServerConnected = socketServer.Listen(5);
  //std::cout << "\nServer IP(s) :  "<< osaSocket::GetLocalhostIP(IPaddress) <<" " <<IPaddress <<" : " << port <<"\n\n";
  //osaSocket::GetLocalhostIP(IPaddress);
  //std::cout<<"IPaddress: "<<IPaddress[0]<<std::endl;
}
void polarisTCPServerTask::Startup(void){
	StateTable.Advance();
}
void polarisTCPServerTask::Run(void){
  ProcessQueuedEvents();
  ProcessQueuedCommands();

  if (!isServerConnected) {
    connectionEstablished = false;
    return;
  }   
  if (isServerConnected && !connectionEstablished) {
    socket = socketServer.Accept();
    if (socket == 0)
      return;
    else 
      connectionEstablished = true;
  }
  if (socket->IsConnected()) {
    //std::cerr<<"receiving";

    std::cout<<"Server Connected!!"<<std::endl;
    bytesRead = socket->Receive((char *)&buffer, sizeof(buffer), 1*cmn_s); 
    if (bytesRead > 0 &&(buffer.toolType==2 || buffer.toolType==4)) {
      vct4 quaternionInput(buffer.qx,buffer.qy,buffer.qz,buffer.qw);
      if(quaternionInput.SumOfElements()!=0.0)
	markerMatrix.Rotation().FromNormalized(vctQuatRot3(buffer.qx,buffer.qy,buffer.qz,buffer.qw, true));
      markerMatrix.Translation().Assign(buffer.translation);
      if(buffer.toolType==2){
	F_RobotTip.Assign(markerMatrix);
      }
      if(buffer.toolType==4)
	F_RobotBase.Assign(markerMatrix);
    }
    return;
  }
  connectionEstablished = false;	
}
void polarisTCPServerTask::Cleanup(void){
	socketServer.Close();  
	delete socket; 
}
void polarisTCPServerTask::CloseSocket(){
	if (socket) socket->Close();
}
