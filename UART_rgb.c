#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
char x;
char z[] = "Enter Inputs R/r G/g or B/b to turn on/off LED:";
char error_msg[] = " is an invalid input, try again. ";
char R_on[] = " ==> Red LED: ON||"; 
char B_on[] = " ==> Blue LED: ON||"; 
char G_on[] = " ==> Green LED: ON||"; 
char R_off[] = " ==> Red LED: OFF||"; 
char B_off[] = " ==> Blue LED: OFF||"; 
char G_off[] = " ==> Green LED: OFF||"; 


void UARTIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status); 				//clear the asserted interrupts

		x= UARTCharGet(UART0_BASE);
		SysCtlDelay(800);
    UARTCharPut(UART0_BASE, x);
		SysCtlDelay(800);
	
		switch(x){
				case 'R':
            for(int i=0; i< sizeof(R_on); i++){
                    UARTCharPut(UART0_BASE, R_on[i]);}
		    UARTCharPut(UART0_BASE, '\r');   
		    UARTCharPut(UART0_BASE, '\n');
            break;
				
				case 'r':
            for(int i=0; i< sizeof(R_off); i++){
                    UARTCharPut(UART0_BASE, R_off[i]);}
		    UARTCharPut(UART0_BASE, '\r');   
		    UARTCharPut(UART0_BASE, '\n');
            break;
				
				case 'B':
            for(int i=0; i< sizeof(B_on); i++){
                    UARTCharPut(UART0_BASE, B_on[i]);}
		    UARTCharPut(UART0_BASE, '\r');   
		    UARTCharPut(UART0_BASE, '\n');
            break;
				
				case 'b':
            for(int i=0; i< sizeof(B_off); i++){
                    UARTCharPut(UART0_BASE, B_off[i]);}
		    UARTCharPut(UART0_BASE, '\r');   
		    UARTCharPut(UART0_BASE, '\n');
            break;
				
				case 'G':
            for(int i=0; i< sizeof(G_on); i++){
              UARTCharPut(UART0_BASE, G_on[i]);}
		    UARTCharPut(UART0_BASE, '\r');   
		    UARTCharPut(UART0_BASE, '\n');
            break;
				
				case 'g':
            for(int i=0; i< sizeof(G_off); i++){
              UARTCharPut(UART0_BASE, G_off[i]);}
		    UARTCharPut(UART0_BASE, '\r');   
		    UARTCharPut(UART0_BASE, '\n');
            break;
								
				default:
            for(int i=0; i< sizeof(error_msg); i++){
							UARTCharPut(UART0_BASE, error_msg[i]);}
						    UARTCharPut(UART0_BASE, '\r');   
		    UARTCharPut(UART0_BASE, '\n');
						break;
			}
}
int main(void) {
	
		SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); //enable pin for LED PF1
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); //enable pin for LED PF2
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); //enable pin for LED PF3

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
		
		for(int i=0; i< sizeof(z); i++)
		{
			UARTCharPut(UART0_BASE, z[i]);
    }
			    UARTCharPut(UART0_BASE, '\r');   
		    UARTCharPut(UART0_BASE, '\n');
	
		
    while (1) //let interrupt handler do the UART echo function
    {
			
			switch(x){ // depending on the user input, the LED will either be on/OFF
				case 'R': // RED LED ON
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
					break;
				
				case 'r': // RED LED OFF
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
					break;
				
				case 'B': // BLUE LED ON
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
					break;
				
				case 'b': // BLUE LED OFF
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
					break;
				
				case 'G': // GREEN LED ON
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
					break;
				
				case 'g': // GREEN LED OFF
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
					break;
			}				
    }
	}
