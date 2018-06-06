import winsound
import time
import sys
import numpy as np
import socket
import os
from random import randint

filename = "../Logs/SoundLogs/Sound_" + time.strftime("%Y%m%d-%H%M%S") + ".txt"
f = open(filename,'a')   # Trying to create a new file or open one
# f.close()

current_milli_time = lambda: int(round(time.time() * 1000))

start_time = current_milli_time()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_addr = ("localhost", 8892)
s.bind(server_addr)
s.listen(1)
conn, client_addr = s.accept()

#s.setblocking(0)
print ("connected\n")
start_time = current_milli_time()
f.write("connected time: " + str(start_time) + "\n")
try:
    print("trying to get data\n")
    signal = conn.recv(1)
    res = "".join(map(chr, signal))
    print("received data: " + res + "\n")
    if res == "2":
        print ("started")
        conn.close()
        nicsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        nicsocket.connect(('localhost', 1234))
        while (True):
            timer = randint (60, 85)
            timer_in_milis = timer * 1000
            while (current_milli_time() - start_time < timer_in_milis):
                continue
            nicsocket.send(b'<TRIGGER>4</TRIGGER>')
            start_time = current_milli_time()
            winsound.PlaySound('../media_resources/beep.wav', winsound.SND_FILENAME)
            print ("beeped at: " + str(start_time))
            f.write("beeped at: " + str(start_time) + "\n")
except(KeyboardInterrupt, SystemExit, socket.error):
    f.close()
    print("exiting")
    sys.exit()
