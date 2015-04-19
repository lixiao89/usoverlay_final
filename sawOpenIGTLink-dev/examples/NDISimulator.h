#include <cisstCommon/cmnPath.h>
#include <cisstCommon/cmnUnits.h>
#include <cisstVector/vctFixedSizeVectorTypes.h>
#include <cisstOSAbstraction/osaStopwatch.h>
#include <cisstMultiTask/mtsMatrix.h>
#include <cisstMultiTask/mtsTaskPeriodic.h>
#include <cisstParameterTypes/prmPositionCartesianGet.h>

class NDISimulator : public mtsTaskPeriodic{

 public:
 NDISimulator(const std::string& taskName, const double period) :
  mtsTaskPeriodic(taskName, period, false, 5000){Construct();}
  ~NDISimulator(void){};

  prmPositionCartesianGet ToolTipPosition;
  prmPositionCartesianGet MarkerPosition;

  double theta;
  
  void Construct(void);
  void Startup(void){};
  void Run(void);
  void Cleanup(void){};

};
