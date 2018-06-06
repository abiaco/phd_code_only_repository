import cv2
import socket
import sys
import numpy as np
import os
import time
from random import randint

current_milli_time = lambda: int(round(time.time() * 1000))

def load_images():
	images = []
	files = os.listdir("images/")
	for image in files:
		img = cv2.imread("images\\" + image, 0)
		images.append(img)
	return files, images


def select_image(current_image, imgno):
	index = randint(0, imgno - 1)
	while index == current_image:
		index = randint(0, imgno - 1)
	return index


def change_image(w1, current_image, images):
	img = select_image(current_image, len(images))
	print ("switch to image: " + str(img) + " named: " + files[img])
	cv2.imshow(w1, images[img])
	cv2.waitKey(1)
	return img

if __name__ == "__main__":
	filename = "../Logs/ImageLogs/Image_" + time.strftime("%Y%m%d-%H%M%S") + ".txt"
	f = open(filename,'a')   # Trying to create a new file or open one

	files, images = load_images()
	imgno = len(images)

	window1 = cv2.namedWindow("Window1")
	cv2.moveWindow("Window1", 1920 , 0)
	cv2.resizeWindow("Window1", 500, 500)

	current_image = change_image("Window1", 0, images);

	accepting_conn = True
	connected = False
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	server_addr = ("localhost", 8893)
	s.bind(server_addr)
	s.listen(1)
	conn, client_addr = s.accept()
	connected = True
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

			while True:
				timer = randint (60, 75)
				timer_in_milis = timer * 1000
				while (current_milli_time() - start_time < timer_in_milis):
					continue
				nicsocket.send(b'<TRIGGER>3</TRIGGER>')
				current_image = change_image("Window1", current_image, images);
				start_time = current_milli_time()
				f.write("changed image to " + files[current_image] + " at: " + str(start_time) + "\n")
				
	except (KeyboardInterrupt, SystemExit, socket.error):
		print ("conn closed")
		f.close()
		exit(0)

		
	f.close()
	exit(0)