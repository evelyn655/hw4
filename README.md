# Homework 4 XBee, BB Car, OpenMV

## To setup and run the program 
### [ Part I - XBee Controlled BB Car]
![](https://i.imgur.com/ZmGqNma.jpg)
如上圖。                                     
在 python 端會透過 Xbee 發送 RPC 指令 ```s.write("/RPC_Parking/run 10 5 U \n".encode())```
輸入的三個參數分別為圖中的 d1、d2、face, 接著 BBcar 便會開始自動倒車。
#### main.cpp
接收到 Xbee 傳來的 RPC 指令後，會進入以下 RPC function:
```
void RPC_Parking(Arguments *in, Reply *out)   {
    double d1 = in->getArg<double>();
    double d2 = in->getArg<double>();
    char face = in->getArg<char>();

    if (face=='S') {
        // go back d2 cm
        encoder_ticker.attach(&encoder_control, 10ms);
        steps = 0;
        last = 0;
        car.goStraight(-50);

        while(steps*6.5*3.14/32 < (d2-5+7)) {
            printf("encoder = %d\r\n", steps);
            ThisThread::sleep_for(100ms);
        }
        car.stop();
    } else {
        // go back d1 cm
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
            car.turn(50, -0.3);
            ThisThread::sleep_for(2500ms);
            car.stop();
        }
        ThisThread::sleep_for(3000ms);
        
        // go back d2 cm
        encoder_ticker.attach(&encoder_control, 10ms);
        steps = 0;
        last = 0;
        car.goStraight(-50);

        while(steps*6.5*3.14/32 < (d2-5+15.5+11)) {
            printf("encoder = %d\r\n", steps);
            ThisThread::sleep_for(100ms);
        }
        car.stop();
    }
        return;
}
```
(1) 首先判斷是否需要轉彎（若 face = 's' 則不須轉彎），若不需要轉彎則直接後退          
(2) 依據輸入的參數， 先使 BBcar 退後一定距離（d1-5+17）          
(3) 依據輸入的參數 face，決定轉彎的方向（轉彎 90度）              
(4) 最後退後（d2-5+15.5+11）的距離                       
* 使用 encoder 測量 BBcar 行走的距離，超過一定距離則停下
* 因為 BBcar 的啟動停下有做緩衝不會急停，所以實際行走的距離需要做實驗調整。

### [ Part II - Line Following BB Car]
如果 BBcar 透過 openMV 的鏡頭看到目標直線就前進，否則停下。
#### openMV - main.py
 openMV 端主要整理出畫面中希望車子 follow 的那條線：
```
if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...
    see_line = 0
    for l in img.find_line_segments(merge_distance = 0, max_theta_diff = 5):
        if l.magnitude() > 12:
            if l.y2() < 50 and abs(l.y2()-l.y1()>10) :
                if l.x1()>20 and l.x2()<100:
                    img.draw_line(l.line(), color = (255, 0, 0))
                    print(l)
                    see_line = 1
                    #uart.write(("L").encode())
                    #time.sleep(0.5)
    uart.write(("%d" % see_line).encode()) # 1: see line, 0: no line
    time.sleep(1)
```
* 透過 magnitude、x1、x2、y1、y2 等數值，可以乾淨的篩選出畫面出希望 BBcar 跟著前進的那條線
* 根據畫面中有沒有看到符合條件的線，將 see_line 的值 write 到 uart 介面，在 mbed 端可以根據 read 到的值決定 BBcar 的運動模式。

#### mbed - main.cpp
```
while(1){
    if(uart.readable()){
        char recv[1];
        uart.read(recv, sizeof(recv));  // &recv[0]
        if (recv[0] == '1') {   // see line, go straight
            printf("straight\r\n");
            car.goStraight(50);
            ThisThread::sleep_for(500ms);
            car.stop();
        } else if (recv[0] == '0') {   // stop
            printf("stop\r\n");
            car.stop();
            ThisThread::sleep_for(500ms);
        } 
        ThisThread::sleep_for(500ms);
     }
}
```
(1) 若 read 到的值為 1，則車子往前走 0.5s        
(2) 每一秒 read 一次數值（所以 main.py 中也配合每 1s 才 write 一次數值），一小段一小段前進，直到畫面中的看不到線為止



### [ Part III - BB Car Position Calibration]
透過 openMV的鏡頭判斷 apriltag 目前相對於 BBcar 是在左邊還右邊，以此控制 BBcar 的移動和轉向。
#### openMV - main.py
```
for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # defaults to TAG36H11
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        if degrees(tag.y_rotation()) <= 5 or (degrees(tag.y_rotation()) >=355 and degrees(tag.y_rotation())<=360):
            uart.write(("s").encode())      # go straight
            time.sleep(2);
        elif (degrees(tag.y_rotation()) >=180 and degrees(tag.y_rotation())<355):
            uart.write(("r").encode())      # slightly turn right (the car seen from the right)
            time.sleep(2);
        elif (degrees(tag.y_rotation()) >5 and degrees(tag.y_rotation())<180):
            uart.write(("l").encode())      # slightly turn left (the car seen from the left)
            time.sleep(2);
```
* 依據 tag.y_roataion() 的值將 apriltag 和 BBcar 的相對位置分成三類：
    * 大約在正前方（355°-5°） - write 's' 到 uart 介面
    * 在鏡頭畫面中偏右（355°-180°） - write 's' 到 uart 介面
    * 在鏡頭畫面中偏左（5°-180°） - write 'l' 到 uart 介面
#### mbed - main.cpp
```
parallax_ping  ping1(pin10);
while(1){
    if(uart.readable()){
        char recv[1];
        uart.read(recv, sizeof(recv));  // &recv[0]
            
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
```
* 首先會由 ping 進行測距，如果和前方障礙物距離 30 公分以內，車子就會停下來
* 若收到 openMV 傳來的 's'，則 BBcar 往前走 1.5s
* 若收到 'r'（代表 BBcar 現在偏右），則讓 BBcar 很小幅度的右轉 1.5s，慢慢修正，左邊同理
* 每2秒 read 一次數值（所以 main.py 中也配合每 2s 才 write 一次數值），一小段一小段前進、修正，直到 BBcar 慢慢轉正、面對 apriltag，接著就會直線前進。

## The results
### [ Part I - XBee Controlled BB Car]

透過 XBee 傳入 RPC 指令: ```RPC_Parking/run/20 12 D```                  
https://drive.google.com/file/d/1H1EXxDf-jbwpT6KMEBlY8hKDsJ5mh3vt/view?usp=sharing

透過 XBee 傳入 RPC 指令: ```RPC_Parking/run/10 5 U```                     
https://drive.google.com/file/d/13ogOfTpvoqTh4S40ZuZaGwVBO8Kg5xht/view?usp=sharing

### [ Part II - Line Following BB Car]
https://drive.google.com/file/d/18oiqZJiOyLQcF-O3q68AS2KjDaVcDHYi/view?usp=sharing

### [ Part III - BB Car Position Calibration]
從左邊:                    
https://drive.google.com/file/d/11s4afld8D_PoE5toAtP5I_sJIfFy6FcD/view?usp=sharing

直走:                      
https://drive.google.com/file/d/1U8NzhWJ8rTMLmyIzJcWG_tIuOXG8ZLu3/view?usp=sharing
