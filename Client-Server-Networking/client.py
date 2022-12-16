import socket

IP = "10.30.143.25"
PORT = 4433
ADDR = (IP, PORT)
SIZE = 1024
FORMAT = "utf-8"

def input_val():
    name = str(input("Enter fighter name: "))
    while (len(name) >= 10):
        name = str(input("Name must be less than 10 characters. Try again: "))

    name_len = str(len(name))
    # add input validation for name length

    attack = int(input("Enter fighter attack stat: "))
    while (attack > 80 or attack < 10):
        attack = int(input("Attack must be between 10 and 80. Try again: "))

    dodge = int(input("Enter fighter dodge stat: "))
    while (dodge > 80 or dodge < 10):
        dodge = int(input("Dodge must be between 10 and 80. Try again: "))

    luck = 100 - attack - dodge
    if (luck > 80 or luck < 10):
        print("Luck must be greater than 10 and less than 80. Try again.")
        return input_val()

    else:
        print("Luck stat: {}".format(luck))

        send_data = name_len + name + str(attack) + str(dodge) + str(luck)
        return (name, send_data)

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

        (name, send_data) = input_val()

        # Send data from the client, receive data, and immediately close
        send_data += "\n\0"
        client.send(send_data.encode(FORMAT))
        recv = client.recv(SIZE).decode(FORMAT)
        if (recv[:4] == "TIE!"):
            (tie, winner, loser) = recv.split()
            if (name == winner):
                print("[MATCHUP] {} vs. {}".format(name, loser))
                print("[RESULTS] {} and {} TIED!".format(name, loser))
            elif (name == loser):
                print("[MATCHUP] {} vs. {}".format(name, winner))
                print("[RESULTS] {} and {} TIED!".format(name, winner))
        else:
            (winner, loser) = recv.split()

        if (bool(recv)):
            if (name == winner):
                print("[MATCHUP] {} vs. {}".format(name, loser))
            elif (name == loser):
                print("[MATCHUP] {} vs. {}".format(name, winner))

            print("[RESULTS] The winner is {}!".format(winner))
        else:
            print("Unable to enter battle.")

        
        client.close()
        print("[DISCONNECTED] Disconnected from the server.")

if __name__ == "__main__":
    main()