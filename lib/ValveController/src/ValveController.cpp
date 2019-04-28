#include "ValveController.h"

ValveController::ValveController() {
    prefs->registerConfigParam("openPin", "Ventil-Öffnen-Pin", "19", 3, this);
    prefs->registerConfigParam("closePin", "Ventil-Schliessen-Pin", "18", 3, this);   

    setup();
}

void ValveController::setup() {
    openPin = prefs->getInt("openPin");
    closePin = prefs->getInt("closePin");    

    pinMode(openPin, OUTPUT);
    pinMode(closePin, OUTPUT);
}

void ValveController::every10Milliseconds() {
    // adjust 2-point regulation
    if (motorAdjustCounter > 0) {
        digitalWrite(openPin, HIGH);
        digitalWrite(closePin, LOW);
        if ((motorAdjustCounter % VALVE_ONE_PERCENT_OPEN_CYCLES) == 0 && valveCurrent < 100) {
            valveCurrent++;
        }
        motorAdjustCounter--;    
    } else if (motorAdjustCounter < 0) {
        digitalWrite(openPin, LOW);
        digitalWrite(closePin, HIGH);
        if ((motorAdjustCounter % VALVE_ONE_PERCENT_OPEN_CYCLES) == 0 && valveCurrent > 0) {
            valveCurrent--;
        }
        motorAdjustCounter++;
    } else {
        // keep current valve position
        digitalWrite(openPin, LOW);
        digitalWrite(closePin, LOW);
    }
}

void ValveController::setTargetValvePosition(int valveTarget) {
    Log.notice("Ventil Ziel: %d\n", valveTarget);

    // if completely open or closed, make sure it is really completely open/closed.
    if (valveTarget == 100 && valveCurrent < 100) {
        valveTarget = 103;
    } else if (valveTarget == 0 && valveCurrent > 0) {
        valveTarget = -3;
    }
    motorAdjustCounter = max(-103.0, min(103.0, (double)(valveTarget - valveCurrent))) * VALVE_ONE_PERCENT_OPEN_CYCLES;
    Log.notice("MotorAdjustCounter: %d\n", motorAdjustCounter);
}


void ValveController::configUpdate(const char *id, const char *value) {
}

double ValveController::getValveCurrent() {
    return valveCurrent;
}

int ValveController::getValveTarget() {
    return valveTarget;
}