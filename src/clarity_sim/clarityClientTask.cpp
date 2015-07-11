#include "clarityClientTask.h"
#include <iostream>
#include <sstream>
#include <math.h>

CMN_IMPLEMENT_SERVICES(clarityClientTask);
clarityClientTask::clarityClientTask(const std::string & taskName, double period, unsigned short cport): mtsTaskPeriodic(taskName, period, false, 1000),socket(osaSocket::TCP){
	isClientConnected = false;
	isConnectionDataReceived = false;
    // ip of server
    host = "10.162.34.218";
	//port = 8089;
	port = cport;
	msglen = 80;
	timeoutSec = 0.05;
	bufsend[0] = 85;
	bufsend[1] = 85;
	bufsend[2] = 85;
	bufsend[3] = 85;
	bufsend[4] = 85;
	bufsend[5] = 85;
	bufsend[7] = 1;
	mtsInterfaceRequired * requiredGUI = AddInterfaceRequired("clarityRequiresGUI");
	if (requiredGUI) {
	  requiredGUI->AddFunction("GetRobotBasePosition", GetRobotBasePosition);
	  requiredGUI->AddFunction("GetRobotTipPosition", GetRobotTipPosition);
	}
}
clarityClientTask::~clarityClientTask(void){}
void clarityClientTask::Configure(const std::string & CMN_UNUSED(filename)){
  isClientConnected = socket.Connect(host.c_str(), port);
}
void clarityClientTask::Startup(void) {}
void clarityClientTask::Cleanup(void){}
void clarityClientTask::Run(void){
    ProcessQueuedEvents();
    ProcessQueuedCommands();
	if(isClientConnected){
	  // Robot Base Position Update
	  GetRobotBasePosition(robotBasePosition);
	  bufsend[6] = 4;
	  FillInBuffer(robotBasePosition);
	  
	  std::cout<<robotBasePosition<<std::endl;
	
	  if(robotBasePosition.Element(0) != 999.9)
	    if(socket.Send(bufsend, msglen, timeoutSec) == -1)
	      std::cerr << "Coordinates not send properly" << std::endl;
	  
	  // Robot Tip Position Update
	  GetRobotTipPosition(robotTipPosition);
	  bufsend[6] = 2;
	  FillInBuffer(robotTipPosition);
	  if(robotTipPosition.Element(0) != 999.9)
	    if(socket.Send(bufsend, msglen, timeoutSec) == -1)
	      std::cerr << "Coordinates not send properly" << std::endl;
	}
}
void clarityClientTask::FillInBuffer(vct7 position){
	time = 0.0;
	char *timeArray = (char*)&time;
	bufsend[8] = timeArray[0];
	bufsend[9] = timeArray[1];
	bufsend[10] = timeArray[2];
	bufsend[11] = timeArray[3];
	bufsend[12] = timeArray[4];
	bufsend[13] = timeArray[5];
	bufsend[14] = timeArray[6];
	bufsend[15] = timeArray[7];

	qx = position.Element(4);
	char *qxArray = (char*)&qx;
	bufsend[16] = qxArray[0];
	bufsend[17] = qxArray[1];
	bufsend[18] = qxArray[2];
	bufsend[19] = qxArray[3];
	bufsend[20] = qxArray[4];
	bufsend[21] = qxArray[5];
	bufsend[22] = qxArray[6];
	bufsend[23] = qxArray[7];

	qy = position.Element(5);
	char *qyArray = (char*)&qy;
	bufsend[24] = qyArray[0];
	bufsend[25] = qyArray[1];
	bufsend[26] = qyArray[2];
	bufsend[27] = qyArray[3];
	bufsend[28] = qyArray[4];
	bufsend[29] = qyArray[5];
	bufsend[30] = qyArray[6];
	bufsend[31] = qyArray[7];

	qz = position.Element(6);
	char *qzArray = (char*)&qz;
	bufsend[32] = qzArray[0];
	bufsend[33] = qzArray[1];
	bufsend[34] = qzArray[2];
	bufsend[35] = qzArray[3];
	bufsend[36] = qzArray[4];
	bufsend[37] = qzArray[5];
	bufsend[38] = qzArray[6];
	bufsend[39] = qzArray[7];

	qw = position.Element(3);
	char *qwArray = (char*)&qw;
	bufsend[40] = qwArray[0];
	bufsend[41] = qwArray[1];
	bufsend[42] = qwArray[2];
	bufsend[43] = qwArray[3];
	bufsend[44] = qwArray[4];
	bufsend[45] = qwArray[5];
	bufsend[46] = qwArray[6];
	bufsend[47] = qwArray[7];

	tx = position.Element(0);
	char *txArray = (char*)&tx;
	bufsend[48] = txArray[0];
	bufsend[49] = txArray[1];
	bufsend[50] = txArray[2];
	bufsend[51] = txArray[3];
	bufsend[52] = txArray[4];
	bufsend[53] = txArray[5];
	bufsend[54] = txArray[6];
	bufsend[55] = txArray[7];

	ty = position.Element(1);
	char *tyArray = (char*)&ty;
	bufsend[56] = tyArray[0];
	bufsend[57] = tyArray[1];
	bufsend[58] = tyArray[2];
	bufsend[59] = tyArray[3];
	bufsend[60] = tyArray[4];
	bufsend[61] = tyArray[5];
	bufsend[62] = tyArray[6];
	bufsend[63] = tyArray[7];

	tz = position.Element(2);
	char *tzArray = (char*)&tz;
	bufsend[64] = tzArray[0];
	bufsend[65] = tzArray[1];
	bufsend[66] = tzArray[2];
	bufsend[67] = tzArray[3];
	bufsend[68] = tzArray[4];
	bufsend[69] = tzArray[5];
	bufsend[70] = tzArray[6];
	bufsend[71] = tzArray[7];

	bufsend[72] = 0;
	bufsend[73] = 0;
	bufsend[74] = 0;
	bufsend[75] = 0;
	bufsend[76] = 0;
	bufsend[77] = 0;
	bufsend[78] = 0;
	bufsend[79] = 0;
}
