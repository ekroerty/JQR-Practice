import socket
import struct
import os
from os import path
import signal

IP = '127.0.0.1'
PORT = 5555
ADDR = (IP, PORT)
FORMAT = 'utf-8'
SIZE = 1024

CREATE = 1
LOGIN = 2
MSG = 3
DISCONNECT = 4
DELETE = 5
UPDATE = 6
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
    msg = str(input("Message: "))
    msg_len = len(msg)
    chat_format = f'<BB{msg_len}s'
    msg_bytes = bytes(msg, FORMAT)
    to_send = struct.pack(chat_format, MSG, msg_len, msg_bytes)
    client.send(to_send)

def update_pass(client):
    uname = str(input("Username: "))
    password = str(input("Old Password: "))
    new_pass = str(input("New Password: "))
    while len(new_pass) > 25:
        print("Password must be 25 characters or less. Try again")
        password = str(input("New Password: "))

    uname_bytes = bytes(uname, FORMAT)
    pass_bytes = bytes(password, FORMAT)
    new_pass_bytes = bytes(new_pass, FORMAT)
    uname_len = len(uname_bytes)
    pass_len = len(pass_bytes)
    new_pass_len = len(new_pass_bytes)
    msg_format = '<BB10sB25sB25s'
    msg = struct.pack(msg_format, UPDATE, uname_len, uname_bytes, pass_len,
                      pass_bytes, new_pass_len, new_pass_bytes)
    print(msg)
    client.send(msg)
    ack = client.recv(SIZE)

    return handle_packet(ack)

def login_signup(client, opt):
    uname = str(input("Username: "))
    password = str(input("Password: "))

    if CREATE == opt:
        while len(uname) > 10:
            print("Username must be 10 characters or less. Try again")
            uname = str(input("Username: "))

        while len(password) > 25:
            print("Password must be 25 characters or less. Try again")
            password = str(input("Password: "))

    uname_bytes = bytes(uname, FORMAT)
    pass_bytes = bytes(password, FORMAT)
    uname_len = len(uname_bytes)
    pass_len = len(pass_bytes)
    msg_format = '<BB10sB25s'

    msg = struct.pack(msg_format, opt, uname_len, uname_bytes, pass_len, pass_bytes)
    # if opt == CREATE or opt == LOGIN:
    if opt == DELETE:
        # msg = struct.pack(msg_format, DELETE, uname_len, uname_bytes, pass_len, pass_bytes)
        client.send(msg)
        return True

    client.send(msg)
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
            return False
        if opcode == INVALID_UNAME:
            print("Username not found!")
            return False
        if opcode == INVALID_LOGIN:
            print("Invalid password!")
            return False

    if len(msg) == 3: # login success packet
        li_pack_format = '<BBB'
        (pack, _res, num_users) = struct.unpack(li_pack_format, msg)

        print("Login Successful!")
        print(f"Users in the chat room: {num_users}")
        return True

def upload_file(client):
    upload_format = '<BIB100s'
    file_path = str(input("Enter file path and name: "))

    if not path.exists(file_path):
        print(f"{file_path} does not exist!")
        return

    file_name = os.path.basename(file_path)
    file_bytes = bytes(file_name, FORMAT)
    file_name_len = len(file_name)
    file_stats = os.stat(file_path)

    upload = struct.pack(upload_format, FILE_UPLOAD, file_stats.st_size,
                        file_name_len, file_bytes)
    client.send(upload)
    print(f"File size: {file_stats.st_size}")
    print(file_path)

    with open(file_path, "rb") as read_file:
        while True:
            bytes_read = read_file.read(file_stats.st_size)
            if not bytes_read:
                print("EOF")
                break
            client.send(bytes_read)

def download_file(client):
    download_req = f'<BB'
    download_msg = struct.pack(download_req, FILE_DOWNLOAD, REQUEST)
    client.send(download_msg)

    print("Files available for download:")
    files = {}
    counter = 1
    while 1:
        options = client.recv(FILE_SIZE + 2)
        # print(options)

        if 0 == len(options):
            break
        try:
            tup = struct.unpack('<BB256s', options)
            files[counter] = tup[2].decode(FORMAT).strip('\0')
            # print(f"{counter}. {tup[2].decode(FORMAT)}")
            counter += 1
        except struct.error:
            break

    for items in files.items():
        print(f"{items}: {files[items]}")

    print("\n")
    while 1:
        file_ind = int(input((f"Enter number between 1 and {counter - 1}: ")))

        if file_ind in files:
            break

        print("Invalid selection.")


    file_path = files[file_ind]

    print(f"File path: {file_path}")

    # file_path = str(input("Enter file name to download: "))
    download_format = f'<BB{len(file_path)}s'
    file_bytes = bytes(file_path, FORMAT)
    download = struct.pack(download_format, FILE_DOWNLOAD, len(file_bytes), file_bytes)
    client.send(download)


    strip_char = bytes("\0", FORMAT)
    full_path = "../client_downloads/" + file_path

    with open(full_path, "wb") as f_download:
        while True:
            msg = client.recv(SIZE).strip(strip_char)
            f_download.write(msg)

            if not msg:
                print("File successfully downloaded!")
                break

    disconnect(client)
    # client.recv(SIZE)


def recv_msgs(client):
    msg_format = "<BB255s"
    # try:
    while 1:
        msg = client.recv(SIZE)
        tup = struct.unpack(msg_format, msg)
        print(tup[2])
    # except KeyboardInterrupt:
    #     disconnect(client)

def client_interact(client):
    # try:
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
        # elif action == "3":
        #     chat(client)
        elif action == "4":
            disconnect(client)
            return
        else:
            print("Invalid input. Type 1, 2, or 3.")

    # except KeyboardInterrupt:
    #     disconnect(client)

def client_actions(client):
    opt = input("1. SIGNUP\n2. LOGIN\n"
    "3. DELETE ACCOUNT\n4. UPDATE PASSWORD\n\n")

    if opt == "1": #user wants to login
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(ADDR)

        status = login_signup(client, LOGIN)
        if not status:
            client.close()
            return

    elif opt == "2":
        status = login_signup(client, CREATE)
        client.close()
        return

    elif opt == "3":
        status = login_signup(client, DELETE)
        client.close()
        return

    elif opt == "4":
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(ADDR)
        status = update_pass(client)
        # client.close()
        return

    else:
        print("Invalid input. Type 1, 2, 3, or 4.")
        client_actions(client)

    # except KeyboardInterrupt:
    #     if client:
    #         disconnect(client)
    #     return

    client_interact(client)

def disconnect(client):
    disconn_format = '<B'
    goodbye = struct.pack(disconn_format, DISCONNECT)
    client.send(goodbye)
    client.close()
    print("[DISCONNECT] Disconnecting from the server.")

def handler(signum, frame):
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
        client_actions(client)
    except KeyboardInterrupt:
        disconnect(client)
    except EOFError:
        disconnect(client)

if __name__ == '__main__':
    main()
