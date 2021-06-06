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
       int output = -1;
       printf("*********while loop!********\r\n");
    printf("output = %d\r\n", output);
    //printf("readable = %d\r\n",uart.readable());
    //ThisThread::sleep_for(500ms);
        if(uart.readable()){
            
            
            // -1: default,  
            //  0: go straight, 
            //  1: seen from the right(turn left), 
            //  2: seen from the left(turn right) 
            char recv[1];
            uart.read(recv, sizeof(recv));  // &recv[0]
            //pc.write(recv, sizeof(recv));
            //output = atoi(recv);
            printf("char= %c\r\n", recv[0]);

            if (recv[0] == 's') {

                printf("straight\r\n");
                // car.goStraight(50);
                 ThisThread::sleep_for(1s);
                // car.stop();
                
                
                // ping detection to stop the car
                // or at the end, break the while loop?

            } else if (recv[0] == 'r') {   // turn left
                printf("from right\r\n");

                // car.turn(50, 0.5);
                 ThisThread::sleep_for(1s);
                // car.stop();

            } else if (recv[0] == 'l') {   // turn right
                printf("from left\r\n");
                // car.turn(50, -0.5);
                 ThisThread::sleep_for(1s);
                // car.stop();
            } else {
                //car.stop();
                // ThisThread::sleep_for(3000ms);
            }
            ThisThread::sleep_for(1s);
      }
   }
}
// string readLine(){
//       string s =  "";
//       char c;
//       while((c = uart.read()) !='\n'){
//           s += c;
//       }  
//       return s;
// }

// int main(){
//     uart.set_baud(9600);

//     thread.start(callback(&queue, &EventQueue::dispatch_forever));                          // for output on uLCD


//     while(1){
//         if(uart.readable()){
//                 char recv[1];
//                 uart.read(recv, sizeof(recv));
//                 //pc.write(recv, sizeof(recv));
//       }
//    }
// }


// void steering() {
//     string RY = readLine();
    

// }