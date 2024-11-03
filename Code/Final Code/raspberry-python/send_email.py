"""this script is used to send an E-Mail to a receiver email-adress in order to send emergency information to an external
phone or app"""

# author: Andreas Auer

# import the library for sending emails
import smtplib

# connect to smtp server
server = smtplib.SMTP('smtp.gmail.com', 587)

# establish connection
server.ehlo()
server.starttls()
server.ehlo()

# Next, log in to the server
server.login("fssandreasauer", "xozseq-xagfis-3vAbmu")

# Send the mail
msg = "/nHello!" # The /n separates the message from the headers
server.sendmail("fssandreasauer@gmail.com", "andreas.auer03@gmail.com", msg)