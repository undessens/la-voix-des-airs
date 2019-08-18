import io
import subprocess
import codecs
import time

import serial
import serial.tools.list_ports as list_ports

from pythonosc.udp_client import SimpleUDPClient

DEBUG = False

def list_devices():
    ports = list_ports.comports()
    for port in ports:
        print(port.device)

def read_serial(serial, decoder):
    ser_bytes = serial.read(1)  # <class 'bytes'>
    if len(ser_bytes) == 0 :
        return None

    if DEBUG:
        print("DEBUG:", ser_bytes)
    
    decoded_str = decoder.decode(ser_bytes)
    return decoded_str

def send_osc(client, msg, address="/debug"):
    client.send_message(address, msg)
    time.sleep(0.2)

def main(port, brate, encoding="utf-8", enc_err="replace", osc_dest="localhost", osc_port=8000):
    # Serial
    ser = serial.Serial(port, brate, timeout=0)
    ser.flushInput()
    rx_encoder = codecs.getincrementaldecoder(encoding)(enc_err)
    end_char = "\n" if DEBUG else ""

    # Osc
    osc_client = SimpleUDPClient(osc_dest, osc_port)  # Create client

    msg = ""
    while True:
        try:
            new_char = read_serial(ser, rx_encoder)
            if new_char:
                msg += new_char
                print(new_char, end=end_char , flush=True)        
                send_osc(osc_client, new_char)
            # if decoded_str == "\n" :
            #     subprocess.call(["say", msg, "-v", "Thomas"])
            #     msg = ""

        except KeyboardInterrupt:
            print("quitting...")
            break


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description='Read from a serial device and send it to OSC')
    parser.add_argument("-l", "--slist", help='list serial devices', dest="print_devices", action="store_true")
    parser.add_argument('-d', "--sdev", help='the name of the serial device', dest="serial_device")
    parser.add_argument('--debug', help='the name of the serial device', action="store_true")

    args = parser.parse_args()

    if args.print_devices:
        list_devices()
    
    if args.debug:
        DEBUG = True

    if args.serial_device:
        port = args.serial_device
        brate = 38400
        main(port, brate)