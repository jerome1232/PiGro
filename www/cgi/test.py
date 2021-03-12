#!/usr/bin/env python3
# -*- coding: UTF-8 -*-# enable debugging

import cgitb, cgi
import Sock_Client
cgitb.enable()

client = Sock_Client.Sock_Client(location='sock')

print("Content-Type: text/html;charset=utf-8")
print()
print("<html>")
print("<title>Threshold data submitted</title>")
print('<link type="text/css" rel="stylesheet" href="../default.css"/>')

form = cgi.FieldStorage()
data = {}
item = None
for key in form.keys():
   item = form[key].value
   last_char = item[-1]
   if last_char == 'f':
      item = item.rstrip(item[-1])
      item = round((float(item) - 32) * 5.0 / 9.0, 2)
   elif last_char == 'c' or last_char == '%':
      item = item.rstrip(item[-1])
   elif last_char == 'm':
      item = int(item.rstrip(item[-1])) * 60_000

   data[key] = item

to_write = ''
for key in data:
   to_write += str(key)
   to_write += ':'
   to_write += str(data[key])
   to_write += ','

to_write = to_write.rstrip(to_write[-1])

print(to_write)

client.write(to_write)

print('This is a test, python3 works!\n')
print('<a href="../control.html">back</a>')