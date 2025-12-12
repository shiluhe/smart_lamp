#include "pi_control.h"

float Kp = 110.0f;
float Ki = 25.2f;
float integral = 0.0f;

float calculate(uint16_t raw_adc)
{
    float v_sense = (float)raw_adc * VREF / ADC_MAX;
    return v_sense * VOLTAGE_SCALE;
}

void PI_Control(float output_voltage)
{
		float dt = 0.001f; // 1 ms
    float error = TARGET_VOLTAGE - output_voltage;

		float Vin = 24.0f;
		float feedforward = TARGET_VOLTAGE / Vin;
		float ff_pwm = feedforward * 2880.0f;
	
    integral += error * dt;

    float pwm = ff_pwm + Kp * error + Ki * integral;

    if(pwm > 2880.0f){
			pwm = 2880.0f;
			if(error > 0)
				integral -= error * dt * 0.5f;
		} 
    if(pwm < 0){
			pwm = 0;
			if(error < 0)
				integral += error * dt;
		} 
		
		if(integral > 200) integral = 200;
    if(integral < -200) integral = -200;

    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (uint32_t)pwm);
}
