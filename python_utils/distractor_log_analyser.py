import pandas as pd

user_names = ["Hil_13-03-2017", "Mihail_22-03-2017", "Mira_28-03-2017", "Radu_12-12-2016", "Alin_03-05-2017", "Alex_15-06-2017", "Dany_16-06-2017", "Qi_23-06-2017"]

data_path = "../../Data/DataPacks/"
soundlog_path = "SoundLog/SoundLog.csv"
imagelog_path = "ImageLog/ImageLog.csv"

def import_soundlog(username):
	filename = data_path + username + "/" + keylog_path
	soundfile = open(filename, "r")
	return soundfile

def import_imagelog(username):
	filename = data_path + username + "/" + mouselog_path
	imagefile = open(filename, "r")
	return imagefile

def count_sound_distractors(logfile, username):
	count = -1
	for line in file:
		count = count + 1
	f = open(data_path + username + "/SoundLog/sound_distractor_count.txt", "w+")
	f.write(count)
	f.close()

def count_image_distractors(logfile, username):
	count = -1
	for line in file:
		count = count + 1
	f = open(data_path + username + "/ImageLog/image_distractor_count.txt", "w+")
	f.write(count)
	f.close()

def process_files():
	for uname in user_names:
		print ("User: " + uname)
		soundlog = import_soundlog(uname)
		print ("imported soundlog")
		count_distractors(soundlog)
		print ("")
		imagelog = import_imagelog(uname)
		print ("imported imagelog")
		count_distractors(imagelog)
		print ("")
		print ("")

