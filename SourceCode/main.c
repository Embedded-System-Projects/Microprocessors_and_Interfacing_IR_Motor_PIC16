#include <16F877A.h>  // Include the header file for the PIC16F877A micro controller

#fuses HS, NOWDT, NOPROTECT, NOLVP  // Configure fuse settings for the micro controller
#use delay(clock=8000000)  // Set the clock frequency for the delay function

int value;   // Variable to store the ADC value from the IR sensor
int toggle;  // Variable to store the ADC value from the toggle switch

void main() {
    // Configure the 5V power net
    setup_adc(ADC_CLOCK_DIV_32);     // Set up the ADC with a clock divider of 32
    setup_adc_ports(AN0);                     // Configure the ADC port to use the AN0 pin

    // Configure ports
    set_tris_b(0b00000111);  // Set RB0-RB2 as inputs, RB3-RB7 as outputs
    set_tris_c(0b00000000);  // Set all PORTC pins as outputs

    // Configure PWM for motor control
    setup_timer_2(T2_DIV_BY_16, 255, 1);  // Set up Timer2 for PWM generation with a prescaler of 16
    setup_ccp1(CCP_PWM);  // Configure CCP1 for PWM output
    setup_ccp2(CCP_PWM);  // Configure CCP2 for PWM output

    while (true) {  // Main program loop
        // Read IR sensor value and toggle value
        value = read_adc(0);  // Read the ADC value from the IR sensor on AN0
        toggle = read_adc(1);  // Read the ADC value from the toggle switch on AN1

        // Determine motor direction based on toggle value
        if (toggle >= 512) {  // If the toggle switch is on (value >= 512)
            // Motor forward
            set_pwm1_duty(value);  // Set the PWM duty cycle for motor 1
            set_pwm2_duty(0);  // Set the PWM duty cycle for motor 2 to 0 (stop)
        } else {  // If the toggle switch is off (value < 512)
            // Motor stop
            set_pwm1_duty(0);  // Set the PWM duty cycle for motor 1 to 0 (stop)
            set_pwm2_duty(0);  // Set the PWM duty cycle for motor 2 to 0 (stop)
        }

        // Delay to prevent excessive CPU usage
        delay_ms(10);  // Add a 10 millisecond delay to prevent the program from running too fast
    }
}
