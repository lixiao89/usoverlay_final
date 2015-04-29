#ifndef _polarisToolComponent_h
#define _polarisToolComponent_h

#include <cisstMultiTask/mtsComponent.h>
#include <cisstMultiTask/mtsFunctionRead.h>
#include <cisstMultiTask/mtsFunctionWrite.h>
#include <cisstParameterTypes/prmPositionCartesianGet.h>
#include <cisstMultiTask/mtsInterfaceRequired.h>
#include <cisstMultiTask/mtsInterfaceProvided.h>

class polarisToolComponent : public mtsComponent{

    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_ALLOW_DEFAULT);

 public:
    polarisToolComponent(const std::string & reqTaskName, const std::string & provTaskName);
    ~polarisToolComponent(void) {};

    void Configure(const std::string & CMN_UNUSED(filename) = "") {};
  
 protected:

    struct {
        mtsFunctionRead GetPositionCartesian;
    } NDI;
	void GetPosition(vct7 & data) const;
};

CMN_DECLARE_SERVICES_INSTANTIATION(polarisToolComponent);

#endif 
