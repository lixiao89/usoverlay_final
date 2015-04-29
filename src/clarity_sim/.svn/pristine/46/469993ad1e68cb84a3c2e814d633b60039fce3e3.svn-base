#ifndef _guiTask_h
#define _guiTask_h
#include <cisstCommon/cmnClassServices.h>
#include <cisstMultiTask.h>
#undef CISST_EXPORT
#define CISST_EXPORT

#include "ClarityUI.h"

class guiTask : public mtsTaskContinuous {
    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_LOD_RUN_ERROR);
public:
    guiTask(const char *name);
    ~guiTask() {}
    void Run(void);
    void Cleanup(void);
    void Startup(void);
    void Configure(const char *filename);

protected:

    ClarityUI		clarityClientUI;
	mtsBool			trackToggle;
	vct7			robotBasePosition;
	vctFrm3			RobotBaseFrame;
	vctQuatRot3		RobotBaseQuaternion;
	vct3			RobotBaseEulerAngles;
	vct7			robotTipPosition;
	vctFrm3			RobotTipFrame;
	vctQuatRot3		RobotTipQuaternion;
	vct3			RobotTipEulerAngles;

	double			r32, r33, r31, r21, r11; 

	struct {
        mtsFunctionWrite Beep;
        mtsFunctionVoid Initialize;
        mtsFunctionVoid Query;
        mtsFunctionVoid Enable;
        mtsFunctionWrite CalibratePivot;
        mtsFunctionWrite Track;
        mtsFunctionVoid ReportStrayMarkers;
    } NDI;

	struct {
        mtsFunctionRead GetPosition;
    } RobotBase;

	struct {
        mtsFunctionRead GetPosition;
    } RobotTip;
	void GetRobotBasePosition(vct7 & data) const;
	void GetRobotTipPosition(vct7 & data) const;
};

CMN_DECLARE_SERVICES_INSTANTIATION(guiTask)

#endif 
