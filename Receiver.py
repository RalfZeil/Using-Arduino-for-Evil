import serial
import subprocess

# Define the serial port
ser = serial.Serial('COM3', 9600)


while True:

    try:
        data = ser.readline().strip().decode('utf-8')
    except:
        continue

    if 'Happiness' in data:
        print(data)
    if 'shutdown' in data:
        # Send shutdown command
        subprocess.run(['shutdown', '/s', '/t', '1'])
        print('Shutdown')
