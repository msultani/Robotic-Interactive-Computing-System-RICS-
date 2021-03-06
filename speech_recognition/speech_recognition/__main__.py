import speech_recognition as sr
import socket
import threading
import json
import os
import time

r = sr.Recognizer()
m = sr.Microphone()

commands = ["retrieve",
            "rise",
            "down",
            "left",
            "right",
            "forward",
            "backward",
            "stop",
            "open", # claw
            "close" # claw
            ]


def process_text(text):
    words = text.split()
    keep = []
    # awake = False
    for word in words:
        # if word == "echo":
        #     awake = True
        # if awake:
        if word in commands:
            keep.append(word)
    return keep


# See python_ports.py from kccla@umich.edu
def send_message(command_value):
    sender = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sender.connect(("0.0.0.0", 6000))
    #data = json.dumps( {"message_value" : command_value} )
    print("sending " + command_value)
    sender.sendall(command_value)
    sender.close()


try:
    print("A moment of silence, please...")
    with m as source: r.adjust_for_ambient_noise(source)
    print("Set minimum energy threshold to {}".format(r.energy_threshold))
    while True:
        print("Say something!")
        with m as source: audio = r.listen(source)
        print("Got it! Now to recognize it...")
        try:
            # recognize speech using Google Speech Recognition
            value = r.recognize_google(audio)

            # TODO RICS: process and send value
            send = socket.socket()
            f = open('words.txt', 'w')
            # we need some special handling here to correctly print unicode characters to standard output
            if str is bytes:  # this version of Python uses bytes for strings (Python 2)
                print(u"You said {}".format(value).encode("utf-8"))
            else:  # this version of Python uses unicode for strings (Python 3+)
                print("You said {}".format(value))
            # RICS code
            val = format(value)
            valid = process_text(val)
            print(valid)
            for word in valid:
                send_message(word)
                time.sleep(1)
        except sr.UnknownValueError:
            print("Oops! Didn't catch that")
        except sr.RequestError as e:
            print("Uh oh! Couldn't request results from Google Speech Recognition service; {0}".format(e))
except KeyboardInterrupt:
    pass

