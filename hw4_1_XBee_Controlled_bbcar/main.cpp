#include "mbed.h"
#include "mbed_rpc.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
volatile int steps;
volatile int last;
BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);

void RPC_Parking(Arguments *in, Reply *out);
RPCFunction rpcRPC_Parking(&RPC_Parking, "RPC_Parking");

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}


void RPC_Parking(Arguments *in, Reply *out)   {
    double d1 = in->getArg<double>();
    double d2 = in->getArg<double>();
    char face = in->getArg<char>();

    if (face=='S') {

    } else {
        // go back di cm
        encoder_ticker.attach(&encoder_control, 10ms);
        steps = 0;
        last = 0;
        car.goStraight(-50);

        printf("d1 = %f\n", d1);
        printf("d2 = %f\n", d2);
        printf("face = %c\n", face);

        while(steps*6.5*3.14/32 < (d1-5+17)) {
            printf("encoder = %d\r\n", steps);
            ThisThread::sleep_for(100ms);
        }
        car.stop();
        ThisThread::sleep_for(3000ms);

        
        // turning 
        if (face=='U') {
            car.turn(50, 0.3);
            ThisThread::sleep_for(2500ms);
            car.stop();
        } else if (face=='D') {

        }
        
        // go back d2 cm
        encoder_ticker.attach(&encoder_control, 10ms);
        steps = 0;
        last = 0;
        car.goStraight(-50);

        printf("d1 = %f\n", d1);
        printf("d2 = %f\n", d2);
        printf("face = %c\n", face);

        while(steps*6.5*3.14/32 < (d2-5+15.5+11)) {
            printf("encoder = %d\r\n", steps);
            ThisThread::sleep_for(100ms);
        }
        car.stop();

    }



    //car.goCertain_Distance(d1);
    
    //car.turn(speed,turn);
    return;
}


int main() {
    //pc.set_baud(9600);


    // encoder_ticker.attach(&encoder_control, 10ms);
    // steps = 0;
    // last = 0;
    // car.goStraight(-100);
    // while(steps*6.5*3.14/32 < (5/2)) {
    //     // printf("encoder = %d\r\n", steps);
    //     ThisThread::sleep_for(100ms);
    // }
    // car.stop();




   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}