import pandas as pd

user_names = ["Hil_13-03-2017", "Mihail_22-03-2017", "Mira_28-03-2017", "Radu_12-12-2016", "Alin_03-05-2017", "Alex_15-06-2017", "Dany_16-06-2017", "Qi_23-06-2017", "Faye_07-09-2017", "Aidan_07-09-2017"]

data_path = "../../Data/DataPacks/"
keylog_path = "KeyLog/KeyLog.csv"
mouselog_path = "MouseLog/MouseLog.csv"


def import_keylog(username):
	filename = data_path + username + "/" + keylog_path
	cols = ['Key', 'time']
	keydict = pd.read_csv(filename, names=cols)
	return keydict

def import_mouselog(username):
	filename = data_path + username + "/" + mouselog_path
	cols = ['Type', 'PosX', 'PosY', 'time']
	mousedict = pd.read_csv(filename, names=cols)
	return mousedict

def print_keylog(keylog):
	print (len(keylog))
	print (keylog.time[0][-2:])

def calc_aps(keylog, username):
	total = 0
	sm = 1
	num = 1
	current_time = keylog.time[0]

	for i in range(len(keylog)):
		if keylog.time[i] != current_time:
			total += sm
			sm = 0
			num += 1
			current_time = keylog.time[i]
		sm += 1
	total += sm
	aps = total / num
	print ("number of seconds: ", num)
	print ("total actions: ", total)
	print ("average aps: ", aps)
	f = open(data_path + username + "/KeyLog/keyscore.txt", "w+");
	f.write(str(aps));
	f.close();

def calc_m_aps(mouselog, username):
	total = 0
	sm = 1
	num = 1
	current_time = mouselog.time[0]
	for i in range(len(mouselog)):
		if mouselog.Type[i] != "MOVE":
			if mouselog.time[i] != current_time:
				total += sm
				sm = 0
				num += 1
				current_time = mouselog.time[i]
			sm += 1
	total += sm
	aps = total / num
	print ("number of seconds: ", num)
	print ("total actions: ", total)
	print ("average aps: ", aps)
	f = open(data_path + username + "/MouseLog/mousescore.txt", "w+");
	f.write(str(aps));
	f.close();

def process_files():
	for uname in user_names:
		print ("User: " + uname)
		keylog = import_keylog(uname)
		print ("imported keylog")
		calc_aps(keylog, uname)
		print ("")
		mouselog = import_mouselog(uname)
		print ("imported mouselog")
		calc_m_aps(mouselog, uname)
		print ("")
		print ("")


if __name__ == '__main__':
	process_files()