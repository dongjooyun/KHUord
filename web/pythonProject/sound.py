import serial
from playsound import playsound


def serial_start():
    ser = serial.Serial('com5', 115200)
    stop_flag = False
    while not stop_flag:
        if ser.readable():
            res = ser.readline()
            res_decode = res.decode()
            print(res_decode)
            play_chord(res_decode)


def play_chord(res_decode):
    sound_played = True
    if res_decode == 'G\r\n':
        if sound_played:
            playsound('./sound//acoustic-guitar-g.mp3')
            sound_played = False
    elif res_decode == 'A\r\n':
        playsound('./sound/acoustic-guitar-a.mp3')
    elif res_decode == 'C\r\n':
        playsound('./sound/acoustic-guitar-c.mp3')
    elif res_decode == 'D\r\n':
        playsound('./sound/acoustic-guitar-d.mp3')
    elif res_decode == 'E\r\n':
        playsound('./sound/acoustic-guitar-e.mp3')
    else:
        print("Wrong Chord!")
