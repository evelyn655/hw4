import serial
import time

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

print("start sending RPC")
s.write("/RPC_Parking/run 10 5 U \n".encode())
time.sleep(1)
# while True:
    # send RPC to remote
    

    # s.write("/myled2/write 1\r".encode())
    # time.sleep(1)

    # s.write("/myled3/write 1\r".encode())
    # time.sleep(1)

    # s.write("/myled3/write 0\r".encode())
    # time.sleep(1)

    # s.write("/myled2/write 0\r".encode())
    # time.sleep(1)

    # s.write("/myled1/write 0\r".encode())
    # time.sleep(1)

s.close()