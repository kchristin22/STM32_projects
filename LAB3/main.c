#include <stdio.h>
#include "uart.h"
#include "platform.h"
#include "timer.h"
#include <gpio.h>
#include "leds.h"
#include <queue.h>
#include "delay.h"
#include "temp_sensor.h"

static int count_press = 0;
Queue rx_queue;
static int sum_aem = 0;
static int sample_rate = 2; // initialize sample rate to the standard frequency
static int counter_T = 2; // initialize counter equal to sample rate
static int ledToggle = 0;

void timer_isr(){
	
	uart_print("Timer interrupt! \r\n");
	if(counter_T == 1){
		temp_init();
		float temp;
		if(temp_data(&temp)){
			uart_print("Unsuccessful data retrieval");
		}
		else{
			char str[100];
			sprintf(str, "Temperature is: %f and the sample rate is: %d sec \n\r ", temp, sample_rate);
			uart_print(str);
			ledToggle = 0; // clear flag at every temp data read
			if(temp > 25){
				leds_set(1,1,1);
			}
			else if(temp < 20){
				leds_set(0,0,0);
			}
			else{
				ledToggle = 1;
			}
			counter_T = sample_rate; // re-initialize counter
		}
	}
	else{
		counter_T--; // update counter every 1 sec
	}
	
	if(ledToggle){
		gpio_toggle(P_LED_R); // toggle LED every sec
	}
	
}

void uart_rx_isr(uint8_t rx) {
	if(rx != '\r'){
			uart_print((char*)&rx);
        if ((rx_queue.tail + 1) < rx_queue.size && rx > 47 && rx < 58){ //ascii 48-57 -> 0-9
            queue_enqueue(&rx_queue, rx); // Store the received character
        }
        else{
					uart_print("\n\rPlease enter a valid AEM: only numbers under 7 digits allowed \n\r");
					rx_queue.tail = 0; 
					
        }
    }
	else{
		uart_print("\r\n");
		sum_aem = rx_queue.data[rx_queue.tail-1] + rx_queue.data[rx_queue.tail-2] -2*48; // data -> char, ASCII: '0' -> 48
		rx_queue.tail = 0;
		timer_init(1000000); // initialize at 1 sec
		timer_set_callback(timer_isr);
		timer_enable();
		
		
	} 

}

void button_press_isr(int sources) {
	if (sources & (GET_PIN_INDEX(P_SW))) {
		
		count_press++;
		
		uart_print("\r\n");
		if(count_press == 1){
			if(sum_aem == 2){
				sample_rate = 4;
			}
			else{
				sample_rate = sum_aem;
			}
		}
		else if(count_press % 2){//odd
			sample_rate = 3;
		}
		else{ //even
			sample_rate = 4;
		}
	counter_T = sample_rate; // re-initialize counter
	}
}

int main(){
	
	queue_init(&rx_queue, 7);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr);
	uart_enable();
	
	leds_init();
	leds_set(0, 0, 0);
	
	// Set up on-board switch.
	gpio_set_mode(P_SW, PullUp);
	gpio_set_trigger(P_SW, Rising); 
	gpio_set_callback(P_SW, button_press_isr); // when button pressed go to ISR
	
	__enable_irq();
	
	uart_print("Enter your AEM: ");
	while(1){
		__WFI();
	}
	
	
	return 0;
}
