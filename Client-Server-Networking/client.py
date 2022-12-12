import socket

IP = "127.0.0.1"
PORT = 4433
ADDR = (IP, PORT)
SIZE = 1024
FORMAT = "utf-8"

def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client.connect(ADDR)
        print("[CONNECTED] Welcome to the server.")
        print("[WAITING] Waiting for server message...")

    except:
        print("Unable to connect to server. Exiting...")

    recv_data = client.recv(SIZE).decode(FORMAT).strip()

    if (recv_data == "Shutdown"):
        print("[SERVER] {}".format(recv_data))
        client.close()
        print("[DISCONNECTED] Disconnected from the server.")
    else:    
        print("[SERVER] {}".format(recv_data))

        # Send data from the client, receive data, and immediately close
        send_data = input("[YOU] ")
        send_data += "\n\0"
        client.send(send_data.encode(FORMAT))
        recv_ack = client.recv(SIZE).decode(FORMAT).strip()[:8]

        if (recv_ack != "Received"):
            print("[ERROR] Failed to send data to server.")
        
        client.close()
        print("[DISCONNECTED] Disconnected from the server.")

if __name__ == "__main__":
    main()