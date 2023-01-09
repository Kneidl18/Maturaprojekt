import ast
import time
from tkinter import *
# from PIL import *

fotowoltaik = "#353DEA"
ziegel = "#817979"
schinteln = "#907251"
heu = "#28882D"
wiese = "#ABFF87"
teer = "#2E352C"
feldweg = "#9EB998"
door_closed = "#2D9543"
door_open = "#FF0000"

# grenzen: if the values of certain sensors are in a dangerous area
heu_temp_grenze = 40  # maximal erlaubte Heu Temperatur
water_temp_grenze = 2  # minimal erlaubte Wasser Temperatur
water_pressure_grenze = 4  # minumaler Wasserdruck

file_path = r'/home/fsshaus/Dokumente/raspberry-python/FSS_DBMS.txt'


class Programm(Frame):
    status = {0b0000: {"time": time.time(), 0b0101: [0, 0, 5], 0b0110: 0},
              0b0001: {"time": time.time(), 0b0010: [10, 20], 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
              0b0010: {"time": time.time(), 0b0001: 0, 0b0010: 5, 0b0100: 0},
              0b0011: {"time": time.time(), 0b0001: 0, 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
              0b0100: [time.time(), 0], 0b0101: [time.time(), 0], 0b0110: [time.time(), 0],
              0b0111: [time.time(), 0], 0b1000: [time.time(), 0], 0b1001: [time.time(), 0], 0b1010: [time.time(), 0],
              0b1011: [time.time(), 0], 0b1100: [time.time(), 0], 0b1101: [time.time(), 0], 0b1110: [time.time(), 0],
              0b1111: [time.time(), 0]}
    door_places_x = [200, 754, 770, 878, 878, 594, 1105, 1245, 1285, 1340, 1390, 1320, 205, 759, 810, 883, 883, 599,
                     1145, 1250, 1290, 1375, 1420, 1325]
    door_places_y = [770, 770, 725, 630, 545, 545, 465, 350, 580, 510, 140, 200, 810, 810, 730, 713, 608, 608, 470, 390,
                     620, 505, 145, 230]
    door_objects = {}

    def __init__(self, master):
        Frame.__init__(self, master)
        self.pack()
        self.my_canvas = Canvas(root, width=1700, height=900, bg=wiese)
        self.my_canvas.pack(pady=45, side=RIGHT)
        self.create_environment()
        self.create_information()
        self.update_information()

    def create_environment(self):
        # feldwege
        self.my_canvas.create_polygon(0, 830, 759, 830, 883, 730, 960, 730, 759, 900, 0, 1000, fill=feldweg)
        self.my_canvas.create_polygon(0, 830, 10, 830, 10, 710, 200, 613, 600, 613, 600, 540, 200, 540, 0, 637,
                                      fill=feldweg)
        self.my_canvas.create_polygon(883, 730, 883, 540, 900, 540, 1000, 613, 1100, 580, 1150, 550, 1175, 500, 1175,
                                      400, 1237, 475, 1277, 515, 1200, 600, 960, 730, fill=teer)
        self.my_canvas.create_polygon(1237, 475, 1200, 300, 1300, 307, 1440, 350, 1448, 495, 1277, 515, fill=teer)

        # statel
        self.my_canvas.create_rectangle(200, 613, 759, 850, fill=ziegel)
        self.my_canvas.create_rectangle(205, 735, 755, 845, fill=fotowoltaik, outline=fotowoltaik)
        self.my_canvas.create_line(200, 731, 759, 731)
        # heu
        self.my_canvas.create_rectangle(300, 625, 446, 720, fill=heu, outline=heu)
        self.my_canvas.create_rectangle(450, 625, 670, 720, fill=heu, outline=heu)

        # maschinenraum
        self.my_canvas.create_rectangle(594, 540, 883, 613, fill=ziegel)

        # anbau
        self.my_canvas.create_rectangle(759, 613, 883, 730, fill=ziegel)

        # mistlege
        self.my_canvas.create_polygon(10, 710, 10, 830, 200, 850, 200, 710, fill=ziegel)
        self.my_canvas.create_rectangle(10, 715, 170, 830, fill=fotowoltaik, outline=fotowoltaik)

        # haus
        self.my_canvas.create_polygon(1010, 460, 1035, 266, 1258, 280, 1237, 475, fill=schinteln)

        # olter stodel
        self.my_canvas.create_polygon(1478, 680, 1448, 495, 1277, 515, 1300, 700, fill=schinteln)

        # schupfe
        self.my_canvas.create_polygon(1300, 307, 1440, 350, 1480, 160, 1335, 120, fill=schinteln)

        # wassertrophen
        # img = ImageTk.PhotoImage(Image.open("water_drop.jpg"))
        # self.my_canvas.create_image(790, 680, anchor=NW, image=img)

    def place_door(self, door_number, door_status):
        temp = object()
        if door_status:
            temp = self.my_canvas.create_rectangle(self.door_places_x[door_number],
                                                   self.door_places_y[door_number],
                                                   self.door_places_x[door_number + 12],
                                                   self.door_places_y[door_number + 12],
                                                   fill=door_closed)
        if not door_status:
            temp = self.my_canvas.create_rectangle(self.door_places_x[door_number],
                                                   self.door_places_y[door_number],
                                                   self.door_places_x[door_number + 12],
                                                   self.door_places_y[door_number + 12],
                                                   fill=door_open)
        return temp

    def update_door_status(self, door_number, door_status):
        self.my_canvas.delete(self.door_objects[door_number])
        self.door_objects[door_number] = self.place_door(door_number, door_status)

    def update_status(self):
        file = open(file_path, 'r')
        temp = str(file.read())
        file.close()

        temp = temp.split('\n')
        self.status: dict = ast.literal_eval(temp[-2])

    def create_information(self):
        self.gruemet_temp_var = StringVar()
        self.gruemet_temp_lbl = Label(self.my_canvas, textvariable=self.gruemet_temp_var, bg=heu, font=("Arial", 25))
        self.gruemet_temp_lbl.place(x=310, y=650)

        self.heu_temp_var = StringVar()
        self.heu_lbl = Label(self.my_canvas, textvariable=self.heu_temp_var, bg=heu, font=("Arial", 25))
        self.heu_lbl.place(x=500, y=650)

        self.current_var = StringVar()
        self.current_lbl = Label(self.my_canvas, textvariable=self.current_var, font=("Arial", 25), bg=wiese)
        self.current_lbl.place(x=10, y=10)

        self.relaise_var = StringVar()
        self.relaise_lbl = Label(self.my_canvas, textvariable=self.relaise_var, font=("Arial", 25), bg=wiese)
        self.relaise_lbl.place(x=530, y=10)

        self.humidity_var = StringVar()
        self.humidity_lbl = Label(self.my_canvas, textvariable=self.humidity_var, font=("Arial", 25), bg=wiese)
        self.humidity_lbl.place(x=1050, y=10)

        self.water_temp_var = StringVar()
        self.water_temp_lbl = Label(self.my_canvas, textvariable=self.water_temp_var, font=("Arial", 25), bg=ziegel)
        self.water_temp_lbl.place(x=760, y=615)

        self.outside_temp_var = StringVar()
        self.outside_temp_lbl = Label(self.my_canvas, textvariable=self.outside_temp_var, font=("Arial", 25), bg=wiese)
        self.outside_temp_lbl.place(x=510, y=490)

        self.stall_temp_var = StringVar()
        self.stall_temp_lbl = Label(self.my_canvas, textvariable=self.stall_temp_var, font=("Arial", 25),
                                    bg=fotowoltaik)
        self.stall_temp_lbl.place(x=420, y=765)

    # in case of an emergency as: too high temperature or too low waterpressure it sends an emergency-SMS
    # error codes: heu -> 0, gruemet -> 1, water -> 2, sicherung -> 3, door -> 4[]
    def send_emergency_information(self, error_code):
        if error_code == 0:
            pass
        elif error_code == 1:
            pass
        elif error_code == 2:
            pass
        elif error_code == 3:
            pass
        else:
            pass

    def update_information(self):
        self.update_status()
        self.current_var.set('Stromverbrauch\nHaus: ' + str(self.status[0][0b0101][0]) + 'A => ' + str(
            round(self.status[0][0b0101][0] * 0.23, 2)) + 'kW\nStadel: ' + str(
            self.status[0][0b0101][1]) + 'A => ' + str(
            round(self.status[0][0b0101][1] * 0.23, 2)) + 'kW\nFotowoltaik: ' + str(
            self.status[0][0b0101][2]) + 'A => ' + str(round(self.status[0][0b0101][2] * 0.23, 2)) + 'kW\n')

        self.gruemet_temp_var.set(str(self.status[1][2][0]) + 'ºC')
        self.heu_temp_var.set(str(self.status[1][2][1]) + 'ºC')
        if self.status[1][2][1] < heu_temp_grenze:
            self.heu_lbl.config(bg=heu)
        else:
            self.heu_lbl.config(bg='red')
            self.send_emergency_information(0)
        if self.status[1][2][0] < heu_temp_grenze:
            self.gruemet_temp_lbl.config(bg=heu)
        else:
            self.gruemet_temp_lbl.config(bg='red')
            self.send_emergency_information(1)

        self.humidity_var.set('Feuchtigkeiten\nStadel: ' + str(self.status[1][0b0011]['hum']) + '%\nStall: ' + str(
            self.status[3][0b0011]['hum']) + '%')

        self.water_temp_var.set(str(self.status[2][2]) + 'ºC\n' + str(self.status[2][4]) + ' Bar')
        if self.status[2][2] >= water_temp_grenze and self.status[2][4] >= water_pressure_grenze:
            self.water_temp_lbl.config(bg=ziegel)
        else:
            self.water_temp_lbl.config(bg='red')
            self.send_emergency_information(2)

        self.outside_temp_var.set(str(self.status[1][3]['temp']) + 'ºC')
        self.stall_temp_var.set(str(self.status[3][3]['temp']) + 'ºC')

        temp1, temp2, temp3 = 'Nicht in Ordnung', 'Nicht in Ordnung', 'Nicht in Ordnung'
        if (self.status[0][6] & 0b111) == 0b111:
            temp1 = "In Ordnung"
        if (self.status[0][6] >> 3 & 0b111) == 0b111:
            temp2 = "In Ordnung"
        if (self.status[0][6] >> 6 & 0b111) == 0b111:
            temp3 = "In Ordnung"
        self.relaise_var.set('Sicherungen\nHaus: ' + temp1 + '\nStadel: ' + temp2 + '\nFotowoltaik: ' + temp3)
        if temp1 == "In Ordnung" and temp2 == "In Ordnung" and temp3 == "In Ordnung":
            self.relaise_lbl.config(bg=wiese)
        else:
            self.relaise_lbl.config(bg='red')
            self.send_emergency_information(0)

        # doors
        for i in range(12):
            self.door_objects[i] = self.place_door(i, self.status[4+i][1])

        self.update_idletasks()
        self.my_canvas.after(1000, self.update_information)


if __name__ == '__main__':
    root = Tk()
    root.title('FSS')
    # root.iconbitmap(r'FSSLOGOCut.ico')
    root.geometry("1920x1080+0+0")
    # root.resizable(False, False)

    app = Programm(master=root)
    # print(app.door_objects)

    app.mainloop()
    root.destroy()
