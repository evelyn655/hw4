#include"mbed.h"
#include <stdlib.h>
#include "mbed_rpc.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

Ticker servo_ticker;
// Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
// DigitalIn encoder(D11);
// volatile int steps;
// volatile int last;
// BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);
// EventQueue queue(32 * EVENTS_EVENT_SIZE);       
// Thread thread; 
// void steering();

int main(){
    uart.set_baud(9600);
    while(1){
        printf("while loop\r\n");
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));  // &recv[0]
            //pc.write(recv, sizeof(recv));
            //output = atoi(recv);
            printf("char= %c\r\n", recv[0]);

            if (recv[0] == '1') {   // see line, go straight
                printf("straight\r\n");
                car.goStraight(50);
                ThisThread::sleep_for(500ms);
                car.stop();
                
                
                // ping detection to stop the car
                // or at the end, break the while loop?

            } else if (recv[0] == '0') {   // stop
                printf("stop\r\n");
                car.stop();
                ThisThread::sleep_for(500ms);
            } 
            ThisThread::sleep_for(500ms);
      }
   }
}



