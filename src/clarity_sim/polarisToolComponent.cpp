#include "polarisToolComponent.h"

CMN_IMPLEMENT_SERVICES(polarisToolComponent);

polarisToolComponent::polarisToolComponent(const std::string & reqTaskName, const std::string & provTaskName):mtsComponent(reqTaskName){
    
    mtsInterfaceRequired * required = AddInterfaceRequired(reqTaskName);
    if (required) 
       required->AddFunction("GetPositionCartesian", NDI.GetPositionCartesian);
    
	mtsInterfaceProvided * provided = AddInterfaceProvided(provTaskName);
    if (provided) 
		provided->AddCommandRead(&polarisToolComponent::GetPosition, this, "GetPosition", vct7());
}
void polarisToolComponent::GetPosition(vct7 & data) const{
	prmPositionCartesianGet PositionCartesian;
	NDI.GetPositionCartesian(PositionCartesian);
	data.SetAll(0.0);
	if (PositionCartesian.Valid()){
		data.Element(0) = PositionCartesian.Position().Translation().X();
		data.Element(1) = PositionCartesian.Position().Translation().Y();
		data.Element(2) = PositionCartesian.Position().Translation().Z();
		vctQuatRot3 toolOrientation;
		toolOrientation.FromNormalized(PositionCartesian.Position().Rotation());
		data.Element(3) = toolOrientation.W();
		data.Element(4) = toolOrientation.X();
		data.Element(5) = toolOrientation.Y();
		data.Element(6) = toolOrientation.Z();
	}
	else {
		data.Element(0) = 999.9;
	}
}