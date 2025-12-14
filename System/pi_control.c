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

		float feedforward = TARGET_VOLTAGE / VIN;
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

void DO_lamp(){
	GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
	if(state == GPIO_PIN_SET){
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 2880.0f * VlampUpLimit / VIN);
	}
	else
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);	
}

void AO_lamp(float light_value){
	float pwm = 0.0f;
	pwm = light_value * 1440.0f / VREF;
	float up_limit = 2880.0f * VlampUpLimit / VIN;
	
	if(pwm > up_limit){
		pwm = up_limit;
	}
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm);	
}

void baoshan(){
	while(1){
		float up_limit =  2880.0f * VlampUpLimit / VIN;
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, up_limit);
		HAL_Delay(50);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);	
		HAL_Delay(50);
	}
}

void huxi_changliang(){
		GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
	if(state == GPIO_PIN_SET){
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 2880.0f * 12 / VIN);
	}
	else{
	  static float pwm_val = 0.0f;
    static float step = 72.0f;   
    pwm_val += step;
	
		float up_limit = 2880.0f * VlampUpLimit / VIN;
		float down_limit = 2880.0f * 0 / VIN;

    if(pwm_val >= up_limit)
    {
        pwm_val = up_limit;
        step = -step;
    }
    else if(pwm_val <= down_limit)
    {
        pwm_val = down_limit;
        step = -step;
    }
		
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (uint16_t)pwm_val);
		HAL_Delay(24);
	}
}

void huxi(){
	  static float pwm_val = 0.0f;
    static float step = 72.0f;   
    pwm_val += step;
	
		float up_limit = 2880.0f * VlampUpLimit / VIN;
		float down_limit = 2880.0f * 0 / VIN;

    if(pwm_val >= up_limit)
    {
        pwm_val = up_limit;
        step = -step;
    }
    else if(pwm_val <= down_limit)
    {
        pwm_val = down_limit;
        step = -step;
    }
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (uint16_t)pwm_val);
		HAL_Delay(24);
}

void on_lamp(){
	float up_limit =  2880.0f * VlampUpLimit / VIN;
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, up_limit);
		
}

void off_lamp(){
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
}
