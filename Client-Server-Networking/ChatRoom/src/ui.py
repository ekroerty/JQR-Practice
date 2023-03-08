import struct

FILE_SIZE = 256
FORMAT = 'utf-8'

class FileOptions:
    def __init__(self, client):
        self.files = {}
        self.counter = 1
        while 1:
            options = client.recv(FILE_SIZE + 2)
            # print(options)

            if 0 == len(options):
                break
            try:
                tup = struct.unpack('<BB256s', options)
                self.files[self.counter] = tup[2].decode(FORMAT).strip('\0')
                # print(f"{counter}. {tup[2].decode(FORMAT)}")
                self.counter += 1
            except struct.error:
                break

    def display(self):
        for items in self.files.items():
            print(f"{items[0]}. {items[1]}")

        print("\n")

    def validate(self):
        while 1:
            try:
                file_ind = int(input((f"Enter number between 1 and {self.counter - 1}: ")))
            except ValueError:
                print("Must enter a number")
                continue
            except TypeError:
                print("Must enter a number")
                continue

            if file_ind in self.files:
                break

            print("Invalid selection.")

        file_path = self.files[file_ind]
        print(f"File path: {file_path}")
        return file_path
