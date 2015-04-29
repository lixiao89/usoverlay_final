#ifndef _polarisTCPServerTask_h
#define _polarisTCPServerTask_h

#include <string.h>
#include <cisstCommon/cmnLogger.h>
#include <cisstCommon/cmnUnits.h>
#include <cisstOSAbstraction/osaSleep.h>
#include <cisstOSAbstraction/osaSocketServer.h>
#include <cisstMultiTask.h>
#include <time.h>
#include <iostream>
#include <stdint.h>

class polarisTCPServerTask : public mtsTaskContinuous {
    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_LOD_RUN_ERROR);
	public:

    polarisTCPServerTask(const char *name, unsigned short cport);
    ~polarisTCPServerTask() {}
    void Run(void);
    void Cleanup(void);
    void Startup(void);
    void Configure(const std::string & CMN_UNUSED(filename) = "");

	protected:
	struct clarityInfo {
		int padding1;
		short padding2;
	  //__int8 toolType;
	  //__int8 messageVersion;
	  uint8_t toolType;
	  uint8_t messageVersion;
	  double time, qx,qy,qz,qw;
	  vct3 translation;
	  int CRC;
	  int zeros;
	};

	clarityInfo buffer;
	osaSocket * socket;
	osaSocketServer socketServer;
	unsigned short port ;
	int bytesRead;
	bool isServerConnected ;
	bool connectionEstablished ;
	std::vector<std::string> IPaddress;

	void CloseSocket();
	vct4x4 F_RobotBase;
	vct4x4 F_RobotTip;
	vctFrm4x4 markerMatrix;

};

CMN_DECLARE_SERVICES_INSTANTIATION(polarisTCPServerTask)
#endif 
