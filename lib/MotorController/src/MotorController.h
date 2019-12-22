#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

#include <Module.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <ArduinoLog.h>


class MotorController : public Module {
    public:
	    const char *getName();
        virtual void setup();
        virtual void everySecond();

    private:
        uint8_t pwmPin = 27;
        uint8_t pwmChannel = 1;

        uint8_t currentPin = 32;
        esp_adc_cal_characteristics_t *adc_chars;

        uint8_t value = 0;
        bool rising = true;

        uint32_t readVoltage(adc1_channel_t channel);

};


#endif