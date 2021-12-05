import socket
import datetime


def main():
    UDP_IP = "192.168.1.18"
    UDP_PORT = 4210
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))
    while True:
        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
        print(f'{datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")} {addr[0]} ({addr[1]}): {data.decode()}')

if __name__ == "__main__":
    main()
