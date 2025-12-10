#include "pi_control.h"

float Kp = 48.0f;
float Ki = 0.72f;
float integral = 0.0f;

float calculate(uint16_t raw_adc)
{
    float v_sense = (float)raw_adc * VREF / ADC_MAX;
    return v_sense * VOLTAGE_SCALE;
}

void PI_Control(float output_voltage)
{
		float dt = 0.005f; // 5 ms
    float error = TARGET_VOLTAGE - output_voltage;

    integral += error * dt;

    if(integral > 2000) integral = 2000;
    if(integral < -2000) integral = -2000;

    float pwm = Kp * error + Ki * integral;

    if(pwm > 2880) pwm = 2880;
    if(pwm < 0)    pwm = 0;

    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (uint32_t)pwm);
}
