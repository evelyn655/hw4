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
DigitalOut led1(LED1);
DigitalInOut pin10(D10);

BBCar car(pin5, pin6, servo_ticker);

int main(){
    uart.set_baud(9600);
    parallax_ping  ping1(pin10);
    while(1){
        printf("*********while loop!********\r\n");
        // printf("ping = %f\r\n", (float)ping1);
        if(uart.readable()){

            char recv[1];
            uart.read(recv, sizeof(recv));  // &recv[0]
            
            printf("char= %c\r\n", recv[0]);

            printf("%lf cm \r\n",(float)ping1);
            if((float)ping1>30) led1 = 1;
            else {
                led1 = 0;
                car.stop();
                break;
            }
            
            if (recv[0] == 's') {

                car.goStraight(40);
                ThisThread::sleep_for(1500ms);
                car.stop();
                

            } else if (recv[0] == 'r') {   // turn right
               
                car.turn(40, -0.85);
                ThisThread::sleep_for(1500ms);
                car.stop();


            } else if (recv[0] == 'l') {   // turn left
                
                car.turn(40, 0.9);
                ThisThread::sleep_for(1500ms);
                car.stop();
            } 
            ThisThread::sleep_for(500ms);
      }
   }
}






// void steering() {
//     string RY = readLine();
    

// }