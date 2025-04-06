import socket
sock = socket.socket()
host = "127.0.0.1"
port = 8080

# connect to host
sock.connect((host, port))

msg = "Hello, World\n"
msgLen = len(msg)

sock.send(msgLen.to_bytes())
sock.send(msg.encode())

# recv message and decode here 1024 is buffer size. 
#response = sock.recv(1024).decode()

#print (response)

sock.close()
