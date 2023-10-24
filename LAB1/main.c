#include <stdio.h>
#include <uart.h>

extern int hash1(char* str);
extern int hash2(int numHash);
	
int main(void)
{

	char str1[20];
	char str2[20];
	
	char user_str[20];
	int result1 = 0;
	int total_result = 0;
	
	
	printf("Please enter a string\n");
	
	scanf("%s", user_str);
	
	printf("Your string is: %s\n", user_str);
	
	//uart_init(115200);
	//uart_enable();
	
	result1 = hash1(user_str);
	printf("The hash of string is %d\n", result1);
		
	//sprintf(str1, "The hash of string is: %d\n\r", result1);
	//uart_print(str1);
	
	if(result1 > 0 ){
		
		total_result = hash2(result1);
		printf("The factorial is %d\n", total_result);
		
		
		//sprintf(str2, "The factorial is: %d\r\n\r", total_result);
		//uart_print(str2);
		
	}
	else{
		
		result1 = (-1)*result1;
		
		total_result = hash2(result1);
		printf("The factorial of absolute value is %d\n", total_result);
		
		
		//sprintf(str2, "The factorial of absolute value is: %d\r\n\r", total_result);
		//uart_print(str2);
		
	}
	
	return (0);
	
}
