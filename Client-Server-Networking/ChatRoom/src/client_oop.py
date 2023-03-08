import socket
import struct
import signal
from packets import *
from ui import FileOptions

IP = '127.0.0.1'
PORT = 5555
ADDR = (IP, PORT)
FORMAT = 'utf-8'
SIZE = 1024

# CREATE = 1
# LOGIN = 2
# MSG = 3
# DISCONNECT = 4
# DELETE = 5
# UPDATE = 6
FILE_UPLOAD = 7
FILE_DOWNLOAD = 8
FILE_OPTIONS = 9

FILE_SIZE = 256

REQUEST = 1
ACKNOWLEDGE = 11
SERVER_ERROR = 6
INVALID_UNAME = 7
INVALID_LOGIN = 8
FAILURE = 255

def chat(client):
    packet = MessagePacket()
    client.send(packet.packet)

def update_pass(client):
    packet = UpdatePacket()
    client.send(packet.packet)
    ack = client.recv(SIZE)
    return handle_packet(ack)

def login_signup(client, opt):
    packet = InitPacket(opt)
    if opt == DELETE:
        client.send(packet.packet)
        return True

    client.send(packet.packet)
    ack = client.recv(SIZE)

    return handle_packet(ack)


def handle_packet(msg):
    if len(msg) == 10: # signup success packet
        su_pack_format = '<BBQ'
        (pack, _res, _sess_id) = struct.unpack(su_pack_format, msg)
        if pack == FAILURE:
            return False

        print("Account Creation Successful!")
        return True

    if len(msg) == 2: # failure packet
        fail_format = '<BB'
        (pack, opcode) = struct.unpack(fail_format, msg)
        if pack == UPDATE and opcode == ACKNOWLEDGE:
            print("Successfully updated password!")
            return True
        if pack == FAILURE or opcode == SERVER_ERROR:
            print("Unable to login. Try again.")
            # return False
        if opcode == INVALID_UNAME:
            print("Username not found!")
            # return False
        if opcode == INVALID_LOGIN:
            print("Invalid password!")
            # return False

    if len(msg) == 3: # login success packet
        li_pack_format = '<BBB'
        (pack, _res, num_users) = struct.unpack(li_pack_format, msg)

        print("Login Successful!")
        print(f"Users in the chat room: {num_users}")
        return True

    return False

def upload_file(client):
    packet = UploadPacket()
    client.send(packet.packet)
    print(packet.file_path)
    print(f"File size: {packet.file_stats.st_size}")

    packet.upload(client)

def download_file(client):
    packet = DownloadRequestPacket()
    client.send(packet.packet)

    print("Files available for download:")
    options = FileOptions(client)
    options.display()
    file_path = options.validate()

    packet = DownloadFilePacket(file_path)
    client.send(packet.packet)

    
    packet.download(client)

    disconnect(client)

def recv_msgs(client):
    msg_format = "<BB255s"
    while 1:
        msg = client.recv(SIZE)
        tup = struct.unpack(msg_format, msg)
        print(tup[2])

def client_actions(client):
    while 1:
        action = input("\nWhat do you want to do?\n\n"
        "1. UPLOAD FILE\n2. DOWNLOAD FILE\n"
        #"3. SEND MESSAGE\n"
        "3. DISCONNECT\n\n")

        if action == "1":
            upload_file(client)
        elif action == "2":
            download_file(client)
            return
        elif action == "4":
            chat(client)
        elif action == "3":
            disconnect(client)
            return
        else:
            print("Invalid input. Type 1, 2, or 3.")

def client_init(client):
    opt = input("1. SIGNUP\n2. LOGIN\n"
    "3. DELETE ACCOUNT\n4. UPDATE PASSWORD\n\n")

    if opt == "1": #user wants to signup
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(ADDR)

        status = login_signup(client, CREATE)
        client.close()
        return

    if opt == "2":
        status = login_signup(client, LOGIN)
        if not status:
            client.close()
            return
        client_actions(client)
        return

    if opt == "3":
        status = login_signup(client, DELETE)
        client.close()
        return

    if opt == "4":
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(ADDR)
        status = update_pass(client)
        # client.close()
        return

    print("Invalid input. Type 1, 2, 3, or 4.")
    client_init(client)


def disconnect(client):
    packet = DisconnectPacket()
    client.send(packet.packet)
    client.close()
    print("[DISCONNECT] Disconnecting from the server.")

def handler(signum, _frame):
    print(f"Caught signal {signum}.")
    raise KeyboardInterrupt

def main():
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(ADDR)
    except socket.error:
        print('Unable to connect to server. Exiting...\n')
        if client:
            client.close()
        return 0

    signal.signal(signal.SIGQUIT, handler)
    signal.signal(signal.SIGTERM, handler)

    try:
        client_init(client)
    except KeyboardInterrupt:
        disconnect(client)
    except EOFError:
        disconnect(client)

    return 0

if __name__ == '__main__':
    main()
