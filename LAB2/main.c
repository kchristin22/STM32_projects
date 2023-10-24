#include <stdio.h>
#include <uart.h>
#include <queue.h>
#include <platform.h>
#include <gpio.h>
#include "leds.h"


static int count_press = 0;
char string1[100];
Queue rx_queue;

void uart_rx_isr(uint8_t rx) {	
	
	if(rx != '\r'){
        uart_print((char *)&rx);
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
		if(rx_queue.data[rx_queue.tail-1] % 2){ //check if odd
			leds_set(1, 1, 1);
			uart_print("LEDs state changed! switched on \n\r");
		}
		else{
			leds_set(0, 0, 0);
			uart_print("LEDs state changed! switched off \n\r");
		}
		rx_queue.tail = 0; 
		uart_print("Enter your AEM and press enter: ");
	}

}

void button_press_isr(int sources) {
	if (sources & (GET_PIN_INDEX(P_SW))) {
		
		count_press++;
		
		uart_print("\r\n");
		if(gpio_get(P_LED_R)){ //if LED is on
			leds_set(0, 0, 0);
			uart_print("LEDs state changed! switched off \n\r");
		}else{
			leds_set(1, 1, 1); 
			uart_print("LEDs state changed! switched on \n\r");
		}
		sprintf(string1, "The value of counter is: %d \n\r", count_press);
		uart_print(string1);
		
	}
	
}


int main(void) {
	
	queue_init(&rx_queue, 7);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr);
	
	//change uart interrupt priority
	NVIC_SetPriority(USART2_IRQn, 5); 
	
	uart_enable();
	
	// Initialise LEDs.
	leds_init();
	leds_set(0, 0, 0);
	
	
	// Set up on-board switch.
	gpio_set_mode(P_SW, PullUp);
	gpio_set_trigger(P_SW, Rising); 
	gpio_set_callback(P_SW, button_press_isr); //when button pressed go to ISR

	__enable_irq();
	
	
	uart_print("Enter your AEM and press enter: ");
	
	while (1) {
		__WFI();
}
	
	return (0);
	
}








