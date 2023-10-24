#include "platform.h"
#include "temp_sensor.h"
#include "gpio.h"
#include "delay.h"


void temp_init(){
	gpio_set_mode(DATA, Output);
	gpio_set(DATA, 1);
}

bool temp_data(float *data){
	
	gpio_set(DATA, 0); // start signal
	delay_ms(20);
	gpio_set(DATA, 1);
	gpio_set_mode(DATA, Input);
	delay_us(50); // wait for response
	
	if (gpio_get(DATA)){
		return 1; // no response
	}
	else{
		while(!gpio_get(DATA));
		delay_us(90); // move to start of data transmission
		if (gpio_get(DATA)){
			return 1; // error
		}
		else{	

		uint64_t data_bit = 0;
		uint64_t tmp_data = 0;
		
		for (int i=39; i>=0; i--){	
			
			while(!gpio_get(DATA));
			delay_us(35); // > 28us, <28us: bit = 0, >28us && <70us: bit = 1
			
			if(gpio_get(DATA)){ // check if bit is 1
				data_bit = 1;
				while(gpio_get(DATA)); // wait for the bit to end
			}
			else{
				data_bit = 0;			
			}
			data_bit = (data_bit << i) & 0xFFFFFFFFFFFFFFFF; // shift it at the correct position (MSB first)
			tmp_data = (tmp_data | data_bit) & 0xFFFFFFFFFFFFFFFF; // add it to the rest of the data
			
		}
		
		uint8_t RH_int = (uint8_t) (tmp_data >> 32) & 0xFF;  // Extract the most significant byte
		uint8_t RH_dec = (uint8_t) (tmp_data >> 24) & 0xFF;  // Extract the second most significant byte
		uint8_t  T_int = (uint8_t)(tmp_data >> 16) & 0xFF; 
		uint8_t T_dec = (uint8_t) (tmp_data >> 8)& 0xFF;     
		uint8_t check_sum = (uint8_t) tmp_data & 0xFF;			// Extract the least significant byte 
		
		if (check_sum != (RH_int + RH_dec + T_int + T_dec)){ // check if data aren't correct
			return 1;
		}
		else{
			*data = (float) T_int + 0.1f * (float) T_dec; // write data to argument
			return 0;
			
		}
		
		}
	}
}

