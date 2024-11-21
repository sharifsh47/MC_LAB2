#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>

void configPorts();
void configTimer1A();

int main(void)

{
    unsigned int counter = 0;
    configPorts();
    configTimer1A();
    while (1) {
        counter++;
        GPIO_PORTF_AHB_DATA_R = ((counter >> 1) & 0x01) << 4 | (counter & 0x01); // Display bits 0 and 4 of port F
        GPIO_PORTN_DATA_R = ((counter >> 3) & 0x01) << 1 | ((counter >> 2) & 0x01); // Display bits 0 and 1 port N

                // Wait for Timer 1A timeout
                while ((TIMER1_RIS_R & (0x0001)) == 0);

                // Clear the timeout flag
                TIMER1_ICR_R |= (1 << 0);
    }
}
void configPorts(){
    // Enable LED ports N and F
    SYSCTL_RCGCGPIO_R |= 0x1020;
    while (!(SYSCTL_PRGPIO_R & 0x1020));

    // LED pins
    GPIO_PORTN_DEN_R |= 0x03; // Digital enable register, enables pin 0 and 1 from port N
    GPIO_PORTN_DIR_R |= 0x03; // Direction register, set the pins as 0 and 1 from port N as outputs

    GPIO_PORTF_AHB_DEN_R = 0x11; // Enable pins 0 and 4 from port F
    GPIO_PORTF_AHB_DIR_R = 0x11; // Direction register, set pins 0 and 4 from port F as outputs
}

void configTimer1A(){
    // Enables timer1A
    SYSCTL_RCGCTIMER_R |= (1<<1); // SYSTEM CLOCK FOR TIMER 1
    while (!(SYSCTL_PRTIMER_R & (1<<1))); // Wait until timer 1 is activated

    TIMER1_CTL_R &= ~0x0001; // Disable Timer 1A
    TIMER1_CFG_R = 0x4; // 16-bit mode
    TIMER1_TAMR_R = 0x22; // Periodic mode and match enable
    TIMER1_TAPR_R = 123-1; // prescaler PR = ceil(16M/2^16*0.5)
    TIMER1_TAILR_R= 65041-1; // ILR = ceil(16M/123*0.5)
    TIMER1_TAMATCHR_R = 39024-1; // MV = ceil(16M/123*0.3)
    TIMER1_CTL_R |= 0x0001; // Enable Timer 1A
}
