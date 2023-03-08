import struct
import os
from os import path

FORMAT = 'utf-8'

CREATE = 1
LOGIN = 2
MSG = 3
DISCONNECT = 4
DELETE = 5
UPDATE = 6

FILE_UPLOAD = 7
FILE_DOWNLOAD = 8

REQUEST = 1

SIZE = 1024

class InitPacket:
    # signup_format = '<BB10sB25s'

    def __init__(self, opt):
        self.uname = str(input("Username: "))
        self.password = str(input("Password: "))

        if opt == CREATE:
            while len(self.uname) > 10:
                print("Username must be 10 characters or less. Try again")
                self.uname = str(input("Username: "))

            while len(self.password) > 25:
                print("Password must be 25 characters or less. Try again")
                self.password = str(input("Password: "))

        uname_bytes = bytes(self.uname, FORMAT)
        pass_bytes = bytes(self.password, FORMAT)
        self.uname_len = len(uname_bytes)
        self.pass_len = len(pass_bytes)
        msg_format = '<BB10sB25s'

        self.packet = struct.pack(msg_format, opt,
                                  self.uname_len, uname_bytes,
                                  self.pass_len, pass_bytes)

class UpdatePacket:

    def __init__(self):
        self.uname = str(input("Username: "))
        self.password = str(input("Old Password: "))
        self.new_pass = str(input("New Password: "))
        while len(self.new_pass) > 25:
            print("Password must be 25 characters or less. Try again")
            self.password = str(input("New Password: "))

        uname_bytes = bytes(self.uname, FORMAT)
        pass_bytes = bytes(self.password, FORMAT)
        new_pass_bytes = bytes(self.new_pass, FORMAT)
        self.uname_len = len(uname_bytes)
        self.pass_len = len(pass_bytes)
        self.new_pass_len = len(new_pass_bytes)
        msg_format = '<BB10sB25sB25s'
        self.packet = struct.pack(msg_format, UPDATE,
                                  self.uname_len, uname_bytes,
                                  self.pass_len, pass_bytes,
                                  self.new_pass_len, new_pass_bytes)

class UploadPacket:

    def __init__(self):
        self.file_path = str(input("Enter file path and name: "))

        if not path.exists(self.file_path):
            print(f"{self.file_path} does not exist!")
            return

        self.file_name = os.path.basename(self.file_path)
        file_bytes = bytes(self.file_name, FORMAT)
        self.file_name_len = len(self.file_name)
        self.file_stats = os.stat(self.file_path)
        upload_format = '<BIB100s'

        self.packet = struct.pack(upload_format, FILE_UPLOAD, self.file_stats.st_size,
                            self.file_name_len, file_bytes)

    def upload(self, client):
        with open(self.file_path, "rb") as read_file:
            while True:
                bytes_read = read_file.read(self.file_stats.st_size)
                if not bytes_read:
                    print("EOF")
                    break
                client.send(bytes_read)

class DownloadRequestPacket:
    def __init__(self):
        download_req = '<BB'
        self.packet = struct.pack(download_req, FILE_DOWNLOAD, REQUEST)

class DownloadFilePacket:
    def __init__(self, file_path):
        self.file_path = file_path
        download_format = f'<BB{len(file_path)}s'
        file_bytes = bytes(file_path, FORMAT)
        self.packet = struct.pack(download_format, FILE_DOWNLOAD,
                                  len(file_bytes), file_bytes)

    def download(self, client):
        strip_char = bytes("\0", FORMAT)
        full_path = "../client_downloads/" + self.file_path

        with open(full_path, "wb") as f_download:
            while True:
                msg = client.recv(SIZE).strip(strip_char)
                f_download.write(msg)

                if not msg:
                    print("File successfully downloaded!")
                    break

class MessagePacket:
    def __init__(self):
        self.msg = str(input("Message: "))
        self.msg_len = len(self.msg)
        chat_format = f'<BB{self.msg_len}s'
        msg_bytes = bytes(self.msg, FORMAT)
        self.packet = struct.pack(chat_format, MSG, self.msg_len, msg_bytes)

class DisconnectPacket:
    def __init__(self):
        disconn_format = '<B'
        self.packet = struct.pack(disconn_format, DISCONNECT)
