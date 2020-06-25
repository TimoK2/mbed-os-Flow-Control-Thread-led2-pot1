/* Flow control with three threat
* Blinking LEDs in the main and a thread for the led.
* Reading analog input with 12 bits and calculating moving average in a thread for the the potentiometer
* connect:
* D10 - 330 Ohm - LED - GND
* D11 - 330 Ohm - LED - GND
* GND - Pot 1
* A3 - Pot 2
* Vcc - Pot 3
*
* Tested with Nucleo-L432KC but should work with almost any board
*
* Thanks to "mbed-os-Flow-Control_Thread" example and AnalogIn API Reference
* Timo Karppinen 25.6.2020, Apache 2.0
*/ 

#include "mbed.h"

DigitalOut led1(D10);
DigitalOut led2(D11);
AnalogIn pot1(A3);
#define numSamples 1024

Thread threadL;
Thread threadP;

// Thread for the LED2. Sleeps for 1 second.
void led2_thread()
{
    while (true) {
        led2 = !led2;
        printf("Led2 blink %d \n", led2.read());
     
        ThisThread::sleep_for(1000);
    }
}


//Thread for the Pot. Sleeps for one millisecond in the middle of processing.
void pot1_thread()
{
    while(true) {
    uint16_t pot1Samples[numSamples];  //unsigned short in the range [0x0, 0xFFFF], [0, 65535]
    int sum;
    int result;
    
    for (int i = 0; i < numSamples; i++) {
        pot1Samples[i] = pot1.read_u16()/16; // normalized into range [0x0, 0xFFFF] and to 12 bit [0x0, 0xFFF]
        //printf("Pot1 sample %d \n", pot1Samples[i]);  // prints every sample
        ThisThread::sleep_for(1);
        }
    printf("Results: ");  // always the average of last samples
    sum = 0;
    for (int i = 0; i < numSamples; i++) {
        sum = sum + pot1Samples[i];
        }
    result = sum/numSamples;
    printf("%d \n", result); 
    }
}

// The main() is in it's own thread.
int main()
{
    // Create a thread to execute the function led2_thread
    threadL.start(led2_thread);
    // led2_thread is executing concurrently with the main and other threads
    
       
    // Create a thread to execute the function pot1_thread
    threadP.start(pot1_thread);
    // pot1_thread is executing concurrently with the main and other threads
 

    while (true) {
        led1 = !led1;
        printf("Led1 blink %d \n", led1.read());
        ThisThread::sleep_for(500);
    }
}
