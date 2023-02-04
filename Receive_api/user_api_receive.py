import socket
import os
import time

timestr = time.strftime("%Y%m%d-%H%M%S")

number = 0
number_acc = 0
x = 0
number_of_mic = 0
flag = 0
flag_acc = 0
choose_file = 0
nr_of_file = 0
flag_eof_mic = 0
flag_eof_acc = 0


UDP_IP = "192.168.4.2"
UDP_PORT = 15200


os.mkdir(timestr)
mysocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
mysocket.bind((UDP_IP, UDP_PORT))
os.mkdir(f"{timestr}\sample{0}")
while True:

    if flag_eof_mic == 1 and flag_eof_acc == 1:
        nr_of_file = nr_of_file + 1
        os.mkdir(f"{timestr}\sample{nr_of_file}")
        flag_eof_mic = 0
        flag_eof_acc = 0
    data, addr = mysocket.recvfrom(1201)
    x = [data[i : i + 1] for i in range(len(data))]

    if int.from_bytes(x[0], "big") == 0:
        f_mic = open(f"{timestr}\sample{nr_of_file}\Mic_{number}", "wb")
        for number_of_mic in range(0, 80):
            if flag == 1:
                data, addr = mysocket.recvfrom(1201)
                x = [data[i : i + 1] for i in range(len(data))]
            else:
                flag = 1
            if number_of_mic == 79:
                flag = 0
            for i in range(1, 1201):
                f_mic.write(x[i])
        f_mic.close()
        print("file close mic")
        flag_eof_mic = 1
        number = number + 1

    if int.from_bytes(x[0], "big") == 128:
        f_acc_x = open(f"{timestr}\sample{nr_of_file}\Acc_x{number_acc}", "a")
        f_acc_y = open(f"{timestr}\sample{nr_of_file}\Acc_y{number_acc}", "a")
        f_acc_z = open(f"{timestr}\sample{nr_of_file}\Acc_z{number_acc}", "a")
        for number_of_acc in range(0, 12):
            if flag_acc == 1:
                data, addr = mysocket.recvfrom(1201)
                x = [data[i : i + 1] for i in range(len(data))]
            else:
                flag_acc = 1
            if number_of_acc == 11:
                flag_acc = 0
            for i in range(1, 1201, 2):
                r = int.from_bytes(b"".join([x[i], x[i + 1]]), "big", signed=True)
                if choose_file == 0:
                    f_acc_x.write(str(r))
                    f_acc_x.write(",")
                    choose_file = 1
                elif choose_file == 1:
                    f_acc_y.write(str(r))
                    f_acc_y.write(",")
                    choose_file = 2
                elif choose_file == 2:
                    f_acc_z.write(str(r))
                    f_acc_z.write(",")
                    choose_file = 0

        f_acc_x.close()
        f_acc_y.close()
        f_acc_z.close()
        print("file close acc")
        flag_eof_acc = 1
        number_acc = number_acc + 1
