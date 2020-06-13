#include <MotorController.h>

const char *MotorController::getName() {
	return "motor";
}

void MotorController::setup() {
    // initialize pwm output pin
	pinMode(pwm1Pin, OUTPUT);
	pinMode(pwm2Pin, OUTPUT);

    ledcSetup(pwm1Channel, 4000, 8);
    ledcSetup(pwm2Channel, 4000, 8);
    ledcAttachPin(pwm1Pin, pwm1Channel);
    ledcAttachPin(pwm2Pin, pwm2Channel);

    // initialize current pin 
    //Characterize ADC at particular atten
    adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_BIT_12, 3300, adc_chars);

    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_0db);

    Log.notice("coeff_a: %d\n", adc_chars->coeff_a);
    Log.notice("coeff_b: %d\n", adc_chars->coeff_b);
    Log.notice("v_ref: %d\n", adc_chars->vref);
    //Check type of calibration value used to characterize ADC
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        Log.notice("eFuse Vref\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        Log.notice("Two Point\n");
    } else {
        Log.notice("Default\n");
    }    
}

void MotorController::every100Milliseconds() {
    // read current from pin 32
    uint32_t currentVoltage = readVoltage(ADC1_CHANNEL_4);
    Log.notice("current current: %d\n", currentVoltage);
    Log.notice("current value: %d\n", value);

    ledcWrite(pwm1Channel, 255 - value);
    ledcWrite(pwm2Channel, 255);

    if (value == 255) {
        rising = false;
        value--;
    } else if (value == 0) {
        rising = true;
        value++;
    } else {
        if (rising) {
            value++;
        } else {
            value--;
        }
    }
}

uint32_t MotorController::readVoltage(adc1_channel_t channel) {
    int adc = adc1_get_raw(channel);
    return esp_adc_cal_raw_to_voltage(adc, adc_chars);
}
