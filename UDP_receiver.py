import socket
import json 

UDP_IP = ""
UDP_PORT = 5000

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print ("received message:", data)
    json_object = json.loads(data.decode())
    print ("Temperature: ", json_object['temperature'], " - Humidity: ", json_object['humidity'])
    print("<---------------------------------------------------------->\n")