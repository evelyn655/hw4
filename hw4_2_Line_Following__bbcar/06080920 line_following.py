import pyb, sensor, image, time, math

enable_lens_corr = False # turn on for straighter lines...
sensor.reset()
sensor.set_pixformat(sensor.RGB565) # grayscale is faster
sensor.set_framesize(sensor.QQVGA) # 120*160
sensor.skip_frames(time = 2000)
clock = time.clock()

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

# All lines also have `x1()`, `y1()`, `x2()`, and `y2()` methods to get their end-points
# and a `line()` method to get all the above as one 4 value tuple for `draw_line()`.

while(True):
    clock.tick()
    img = sensor.snapshot()
    if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

   # `merge_distance` controls the merging of nearby lines. At 0 (the default), no
   # merging is done. At 1, any line 1 pixel away from another is merged... and so
   # on as you increase this value. You may wish to merge lines as line segment
   # detection produces a lot of line segment results.

   # `max_theta_diff` controls the maximum amount of rotation difference between
   # any two lines about to be merged. The default setting allows for 15 degrees.
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
    print("see_line = %d" % see_line)
    print("FPS %f" % clock.fps())
