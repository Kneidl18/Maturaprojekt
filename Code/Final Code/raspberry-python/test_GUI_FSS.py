import time
from tkinter import *

fotowoltaik = "#353DEA"
ziegel = "#817979"
schinteln = "#907251"
heu = "#28882D"
wiese = "#ABFF87"
teer = "#2E352C"
feldweg = "#9EB998"

root = Tk()
root.title('FSS')
root.iconbitmap(
    r'C:\Users\Kneidl\OneDrive - Bildungsdirektion\Andreas\Oberschule\5. Oberschule 202122\Maturaprjoct\Code\Final Code\raspberry-python\FSSLOGOCut_BW.bmp')
root.geometry("1920x1080")

my_canvas = Canvas(root, width=1700, height=900, bg=wiese)
my_canvas.pack(pady=45, side=RIGHT)

#feldwege
my_canvas.create_polygon(0, 830, 759, 830, 883, 730, 960, 730, 759, 900, 0, 1000, fill=feldweg)
my_canvas.create_polygon(0, 830, 10, 830, 10, 710, 200, 613, 600, 613, 600, 540, 200, 540, 0, 637, fill=feldweg)
my_canvas.create_polygon(883, 730, 883, 540, 900, 540, 1000, 613, 1100, 580, 1150, 550, 1175, 500, 1175, 400, 1237, 475, 1277, 515, 1200, 600, 960, 730, fill=teer)
my_canvas.create_polygon(1237, 475, 1200, 300, 1300, 307, 1440, 350, 1448, 495, 1277, 515, fill=teer)

# statel
my_canvas.create_rectangle(200, 613, 759, 850, fill=ziegel)
my_canvas.create_rectangle(205, 735, 755, 845, fill=fotowoltaik, outline=fotowoltaik)
my_canvas.create_line(200, 731, 759, 731)
# heu
my_canvas.create_rectangle(300, 625, 446, 720, fill=heu, outline=heu)
my_canvas.create_rectangle(450, 625, 670, 720, fill=heu, outline=heu)

# maschinenraum
my_canvas.create_rectangle(594, 540, 883, 613, fill=ziegel)

# anbau
my_canvas.create_rectangle(759, 613, 883, 730, fill=ziegel)

# mistlege
my_canvas.create_polygon(10, 710, 10, 830, 200, 850, 200, 710, fill=ziegel)
my_canvas.create_rectangle(10, 715, 170, 830, fill=fotowoltaik, outline=fotowoltaik)
# canvas.create_line(10, 710, 200, 710)
# canvas.create_line(10, 710, 10, 830)
# canvas.create_line(10, 830, 200, 850)


# haus
# canvas.create_rectangle(1050, 200, 1300, 450, fill=schinteln)
# canvas.create_line(1010, 450, 1010, 470)
# canvas.create_line(1000, 460, 1020, 460)
# canvas.create_line(1035, 256, 1035, 276)
# canvas.create_line(1025, 266, 1045, 266)
# canvas.create_line(1225, 475, 1245, 475)
# canvas.create_line(1237, 465, 1237, 485)
# canvas.create_line(1258, 270, 1258, 290)
# canvas.create_line(1245, 280, 1265, 280)

my_canvas.create_line(1010, 460, 1035, 266)
my_canvas.create_line(1035, 266, 1258, 280)
my_canvas.create_line(1010, 460, 1237, 475)
my_canvas.create_line(1237, 475, 1258, 280)
my_canvas.create_polygon(1010, 460, 1035, 266, 1258, 280, 1237, 475, fill=schinteln)

#olter stodel
# canvas.create_line(1267, 515, 1287, 515)
# canvas.create_line(1277, 505, 1277, 525)
# canvas.create_line(1290, 700, 1310, 700)
# canvas.create_line(1300, 690, 1300, 710)
# canvas.create_line(1438, 495, 1458, 495)
# canvas.create_line(1448, 485, 1448, 505)
# canvas.create_line(1468, 680, 1488, 680)
# canvas.create_line(1478, 670, 1478, 690)

my_canvas.create_line(1478, 680, 1448, 495)
my_canvas.create_line(1478, 680, 1300, 700)
my_canvas.create_line(1300, 700, 1277, 515)
my_canvas.create_line(1277, 515, 1448, 495)
my_canvas.create_polygon(1478, 680, 1448, 495, 1277, 515, 1300, 700, fill=schinteln)

# schupfe
# canvas.create_line(1300, 297, 1300, 317)
# canvas.create_line(1290, 307, 1310, 307)
# canvas.create_line(1440, 340, 1440, 360)
# canvas.create_line(1430, 350, 1450, 350)
# canvas.create_line(1335, 110, 1335, 130)
# canvas.create_line(1325, 120, 1345, 120)
# canvas.create_line(1480, 150, 1480, 170)
# canvas.create_line(1470, 160, 1490, 160)

my_canvas.create_line(1300, 307, 1440, 350)
my_canvas.create_line(1335, 120, 1300, 307)
my_canvas.create_line(1335, 120, 1480, 160)
my_canvas.create_line(1480, 160, 1440, 350)
my_canvas.create_polygon(1300, 307, 1440, 350, 1480, 160, 1335, 120, fill=schinteln)

gruemet_temp_var = StringVar()
gruemet_temp_var.set('10,6 C')
gruemet_temp_lbl = Label(my_canvas, textvariable=gruemet_temp_var, bg=heu, font=("Arial", 25))
gruemet_temp_lbl.place(x=310, y=650)

heu_temp_var = StringVar()
heu_temp_var.set('10,5 C')
heu_lbl = Label(my_canvas, textvariable=heu_temp_var, bg=heu, font=("Arial", 25))
heu_lbl.place(x=500, y=650)

relaise_var = StringVar()
relaise_var.set('Stromverbrauch \n  20A => 4,4kW\n5A => 1,1kW\n 10A => 2,2kW')
current_lbl = Label(my_canvas, textvariable=relaise_var, font=("Arial", 25), bg=wiese)
current_lbl.place(x=10, y=10)

relaise_var = StringVar()
relaise_var.set('Sicherungen')
relaise_lbl = Label(my_canvas, textvariable=relaise_var, font=("Arial", 25), bg=wiese)
relaise_lbl.place(x=350, y=10)

root.mainloop()
