from pythonosc.udp_client import SimpleUDPClient
import time

ip = "192.168.11.105" # coco
ip = "192.168.11.104" # MOI
ip = "192.168.11.112" # PRISCILLA

ip = "127.0.0.1"
port = 12345

client = SimpleUDPClient(ip, port)  # Create client

while True:
    print("Sending to ", ip, "on port", port )
    for i in range(10):
        client.send_message("/activelayer/video/positiony/values", "é")
        time.sleep(1)
