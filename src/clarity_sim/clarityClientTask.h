#ifndef _clarityClientTask_h
#define _clarityClientTask_h

#include <string.h>
#include <iostream>
#include <cisstVector/vctTypes.h>
#include <cisstCommon.h>
#include <cisstOSAbstraction.h>
#include <cisstMultiTask.h>

class clarityClientTask : public mtsTaskPeriodic {
  CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_LOD_RUN_DEBUG);  // log level

 public:
  clarityClientTask(const std::string & taskName, double period, unsigned short cport);
  ~clarityClientTask(void);

  // cisstMultiTask Functions 
  void Configure(const std::string & CMN_UNUSED(filename) = "");
  void Startup(void);
  void Run(void);
  void Cleanup(void);
 protected:
  // Connection Parameters
  std::string host ;
  unsigned short port ;
  bool isClientConnected ;
  bool isConnectionDataReceived ;
  char  bufsend [80];
  unsigned int msglen;
  osaSocket socket;
  double timeoutSec;

  double time;
  double qw, qx, qy, qz;
  double tx,ty,tz;

  mtsFunctionRead GetRobotBasePosition;
  mtsFunctionRead GetRobotTipPosition;
  void FillInBuffer(vct7 position);

  vct7	robotBasePosition;
  vct7	robotTipPosition;
};

CMN_DECLARE_SERVICES_INSTANTIATION(clarityClientTask);

#endif 
