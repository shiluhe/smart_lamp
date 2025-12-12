#include "adc.h"

#define VREF              3.41f
#define ADC_MAX           4095.0f
#define R1                10000.0f
#define R2                50000.0f
#define VOLTAGE_SCALE     ((R1 + R2) / R1)
#define TARGET_VOLTAGE    (12.0f)
   

extern TIM_HandleTypeDef htim1;

float calculate(uint16_t raw_adc);
void PI_Control(float output_voltage);

