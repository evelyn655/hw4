#include"mbed.h"
#include <stdlib.h>

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

// EventQueue queue(32 * EVENTS_EVENT_SIZE);       
// Thread thread; 
// void steering();

int main(){
   uart.set_baud(9600);
   while(1){
        if(uart.readable()){
            int output = -1;
            // -1: default,  
            //  0: go straight, 
            //  1: seen from the right(turn left), 
            //  2: seen from the left(turn right) 
            char recv[1];
            uart.read(recv, sizeof(recv));  // &recv[0]
            //pc.write(recv, sizeof(recv));
            output = atoi(recv);
            printf("output= %d\r\n", output);


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