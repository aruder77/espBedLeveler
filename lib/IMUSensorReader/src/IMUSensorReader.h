#ifndef IMUSensorReader_H_
#define IMUSensorReader_H_

#include <Module.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>


class IMUSensorReader : public Module {
    public:
	    const char *getName();
        virtual void setup();
        virtual void every100Milliseconds();

    private:
        uint8_t pwm1Pin = 26;
        uint8_t pwm1Channel = 1;
        uint8_t pwm2Pin = 27;
        uint8_t pwm2Channel = 2;

        uint8_t currentPin = 32;
        esp_adc_cal_characteristics_t *adc_chars;

        uint8_t value = 0;
        bool rising = true;

        uint32_t readVoltage(adc1_channel_t channel);

};


#endif