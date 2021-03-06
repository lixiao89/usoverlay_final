#include <cisstCommon/cmnPortability.h>
#include <cisstOSAbstraction/osaSleep.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "guiTask.h"

CMN_IMPLEMENT_SERVICES(guiTask);
guiTask::guiTask(const char *name) : mtsTaskContinuous(name){
	mtsInterfaceRequired * required = AddInterfaceRequired("guiRequiresNDI");
    if (required) {
        required->AddFunction("Beep", NDI.Beep);
        required->AddFunction("PortHandlesInitialize", NDI.Initialize);
        required->AddFunction("PortHandlesQuery", NDI.Query);
        required->AddFunction("PortHandlesEnable", NDI.Enable);
        required->AddFunction("CalibratePivot", NDI.CalibratePivot);
        required->AddFunction("ToggleTracking", NDI.Track);
        required->AddFunction("ReportStrayMarkers", NDI.ReportStrayMarkers);
    }
    
	mtsInterfaceRequired * requiredRobotBase = AddInterfaceRequired("guiRequiresRobotBase");
    if (requiredRobotBase) {
        requiredRobotBase->AddFunction("GetPosition", RobotBase.GetPosition);
    }
	mtsInterfaceRequired * requiredRobotTip = AddInterfaceRequired("guiRequiresRobotTip");
    if (requiredRobotTip) {
        requiredRobotTip->AddFunction("GetPosition", RobotTip.GetPosition);
    }

	mtsInterfaceProvided * provided = AddInterfaceProvided("guiProvidesClarity");
	if (provided) {
		provided->AddCommandRead(&guiTask::GetRobotBasePosition, this, "GetRobotBasePosition", vct7());
		provided->AddCommandRead(&guiTask::GetRobotTipPosition, this, "GetRobotTipPosition", vct7());
	}

	trackToggle = true;
	robotBasePosition.SetAll(0.0);
	robotTipPosition.SetAll(0.0);
}
void guiTask::Configure(const char *filename){}
void guiTask::Startup(void){
    clarityClientUI.show(0, NULL);
    Fl::check();

    for (unsigned int i=0; i<3; i++){
      clarityClientUI.BasePosition[i]->precision(3);
      clarityClientUI.BasePosition[i+3]->precision(3);
      clarityClientUI.TipPosition[i]->precision(3);
      clarityClientUI.TipPosition[i+3]->precision(3);
    }
}
void guiTask::Run(void){
	ProcessQueuedEvents();
    ProcessQueuedCommands();

	if(clarityClientUI.beepClicked){
		clarityClientUI.beepClicked = false;
		mtsInt numberOfBeeps = (int)clarityClientUI.beepCount->value();
		NDI.Beep(numberOfBeeps);
	}

	if(clarityClientUI.initializeClicked){
		clarityClientUI.initializeClicked = false;
		NDI.Initialize();
		NDI.Query();
		NDI.Enable();
	}

	if(clarityClientUI.trackClicked){
		clarityClientUI.trackClicked = false;
		NDI.Track(trackToggle);
		trackToggle = !trackToggle;
	}

	RobotBase.GetPosition(robotBasePosition);
	RobotBaseQuaternion.W() = robotBasePosition.Element(3);
	RobotBaseQuaternion.X() = robotBasePosition.Element(4);
	RobotBaseQuaternion.Y() = robotBasePosition.Element(5);
	RobotBaseQuaternion.Z() = robotBasePosition.Element(6);
	RobotBaseFrame.Rotation().FromRaw(RobotBaseQuaternion);
	r32 = RobotBaseFrame.Rotation().Element(2,1);
	r33 = RobotBaseFrame.Rotation().Element(2,2);
	r31 = RobotBaseFrame.Rotation().Element(2,0);
	r21 = RobotBaseFrame.Rotation().Element(1,0);
	r11 = RobotBaseFrame.Rotation().Element(0,0);
	RobotBaseEulerAngles.Element(0) = atan2(r32,r33)*cmn180_PI;
	RobotBaseEulerAngles.Element(1) = atan2(-r31,sqrt(pow(r32,2.0) + pow(r33,2.0)))*cmn180_PI;
	RobotBaseEulerAngles.Element(2) = atan2(r21,r11)*cmn180_PI;

	RobotTip.GetPosition(robotTipPosition);
	RobotTipQuaternion.W() = robotTipPosition.Element(3);
	RobotTipQuaternion.X() = robotTipPosition.Element(4);
	RobotTipQuaternion.Y() = robotTipPosition.Element(5);
	RobotTipQuaternion.Z() = robotTipPosition.Element(6);
	RobotTipFrame.Rotation().FromRaw(RobotTipQuaternion);
	r32 = RobotTipFrame.Rotation().Element(2,1);
	r33 = RobotTipFrame.Rotation().Element(2,2);
	r31 = RobotTipFrame.Rotation().Element(2,0);
	r21 = RobotTipFrame.Rotation().Element(1,0);
	r11 = RobotTipFrame.Rotation().Element(0,0);
	RobotTipEulerAngles.Element(0) = atan2(r32,r33)*cmn180_PI;
	RobotTipEulerAngles.Element(1) = atan2(-r31,sqrt(pow(r32,2.0) + pow(r33,2.0)))*cmn180_PI;
	RobotTipEulerAngles.Element(2) = atan2(r21,r11)*cmn180_PI;

	for (unsigned int i=0; i<6; i++){
	  if(robotBasePosition.Element(0) != 999.9){
	    if(i<3)
	      clarityClientUI.BasePosition[i]->value(robotBasePosition.Element(i));
	    else
	      clarityClientUI.BasePosition[i]->value(RobotBaseEulerAngles.Element(i-3));
	  }
	  else
	    clarityClientUI.BasePosition[i]->value(99999.999);

	  if(robotTipPosition.Element(0) != 999.9){
	    if(i<3)
	      clarityClientUI.TipPosition[i]->value(robotTipPosition.Element(i));
	    else
	      clarityClientUI.TipPosition[i]->value(RobotTipEulerAngles.Element(i-3));
	  }
	  else
	    clarityClientUI.TipPosition[i]->value(99999.999);
	}

	if (Fl::check() == 0)
		Kill();
}
void guiTask::Cleanup(void){}
void guiTask::GetRobotBasePosition(vct7 & data) const{
	data.Assign(robotBasePosition);
}
void guiTask::GetRobotTipPosition(vct7 & data) const{
	data.Assign(robotTipPosition);
}
