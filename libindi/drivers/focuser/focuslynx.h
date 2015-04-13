/*
    Focus Lynx INDI driver
    Copyright (C) 2015 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef FOCUSLYNX_H
#define FOCUSLYNX_H

#include <map>
#include "indibase/indifocuser.h"

class FocusLynx : public INDI::Focuser
{
public:
    FocusLynx();
    ~FocusLynx();

    enum { FOCUS_A_COEFF, FOCUS_B_COEFF, FOCUS_C_COEFF, FOCUS_D_COEFF, FOCUS_E_COEFF, FOCUS_F_COEFF };
    enum { STATUS_MOVING, STATUS_HOMING, STATUS_HOMED, STATUS_FFDETECT, STATUS_TMPPROBE, STATUS_REMOTEIO, STATUS_HNDCTRL };

    virtual bool Connect();
    virtual bool Disconnect();
    const char * getDefaultName();
    virtual bool initProperties();
    virtual void ISGetProperties(const char *dev);
    virtual bool updateProperties();
    virtual bool saveConfigItems(FILE *fp);

    virtual bool ISNewNumber (const char *dev, const char *name, double values[], char *names[], int n);
    virtual bool ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n);

    virtual int MoveFocuser(FocusDirection dir, int speed, int duration);
    virtual int MoveAbsFocuser(int ticks);
    virtual int MoveRelFocuser(FocusDirection dir, unsigned int ticks);
    virtual bool SetFocuserSpeed(int speed);
    virtual bool AbortFocuser();
    virtual void TimerHit();

    void debugTriggered(bool enable);

private:

    int PortFD;
    //double targetPos, lastPos, lastTemperature, simPosition;
    //unsigned int currentSpeed, temperaturegetCounter;

    std::map<std::string, std::string> lynxModels;

    struct timeval focusMoveStart;
    //float focusMoveRequest;

    // Get functions
    void getInitialData();
    bool getFocusConfig();
    bool getFocusStatus();

    // Set functions

    // Device
    bool setDeviceType(int index);

    // Position
    bool setFocusPosition(u_int16_t position);

    // Temperature
    bool setTemperatureCompensation(bool enable);
    bool setTemperatureCompensationMode(char mode);
    bool setTemperatureCompensationCoeff(u_int16_t coeff);
    bool setTemperatureCompensationOnStart(bool enable);

    // Backlash
    bool setBacklashCompensation(bool enable);
    bool setBacklashCompensationSteps(u_int16_t steps);

    // Sync
    bool sync(u_int16_t position);

    // Motion functions
    bool stop();
    bool startMotion(FocusDirection dir);
    bool home();
    bool center();

    // Misc functions
    bool ack();
    bool resetFactory();
    float calcTimeLeft(timeval,float);
    bool isResponseOK();

    // Properties

    // Set/Get Temperature
    INumber TemperatureN[1];
    INumberVectorProperty TemperatureNP;

    // Enable/Disable temperature compnesation
    ISwitch TemperatureCompensateS[2];
    ISwitchVectorProperty TemperatureCompensateSP;

    // Enable/Disable temperature compnesation on start
    ISwitch TemperatureCompensateOnStartS[1];
    ISwitchVectorProperty TemperatureCompensateOnStartSP;

    // Temperature Coefficient
    INumber TemperatureCoeffN[5];
    INumberVectorProperty TemperatureCoeffNP;

    // Enable/Disable backlash
    ISwitch BacklashCompensationS[2];
    ISwitchVectorProperty BacklashCompensationSP;

    // Backlash Value
    INumber BacklashN[1];
    INumberVectorProperty BacklashNP;

    // Reset to Factory setting
    ISwitch ResetS[1];
    ISwitchVectorProperty ResetSP;

    // Go to home/center
    ISwitch GotoS[2];
    ISwitchVectorProperty GotoSP;

    // List all supported models
    ISwitch *ModelS;
    ISwitchVectorProperty ModelSP;

    // Status indicators
    ILight StatusL[7];
    ILightVectorProperty StatusLP;

    // Sync to a particular position
    INumber SyncN[1];
    INumberVectorProperty SyncNP;

};

#endif // FocusLynx_H