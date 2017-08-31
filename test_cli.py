import socket
import random
import time

ADDRESS = "127.0.0.1"
PORT = 8787
MSG = "Hello cpp server"

def simulator():
	x = random.randint(-90, 90)
	y = random.randint(-90, 90)
	z = random.randint(-90, 90)
	return str(x)+","+str(y)+","+str(z)



def main():
	buffer = ""
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	try:
		s.connect((ADDRESS,PORT))
	except Exception as e:
		print e
	while(1):
		print "hell4"
		buffer = str(simulator()) + "\n"
		print buffer
		try:
			s.send(buffer)
		except Exception as e:
			print e
			exit(1)
		time.sleep(0.1)
	print "helloasd"
	s.close()








if __name__ == "__main__":
	main()