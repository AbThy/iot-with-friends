import serial
import time

# Setup Serial port 
ser = serial.Serial('/dev/ttyS0', 230400, timeout=1)
time.sleep(2)

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
except KeyboardInterrupt:
    pass
finally:
    ser.close()