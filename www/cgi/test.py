#!/usr/bin/env python3
# -*- coding: UTF-8 -*-# enable debugging

import cgitb, cgi
cgitb.enable()

print("Content-Type: text/html;charset=utf-8")
print()
print("<html>")
print("<title>Threshold data submitted</title>")
print('<link type="text/css" rel="stylesheet" href="../default.css"/>')
form = cgi.FieldStorage()
data = {}
for key in form.keys():
   data[key] = form[key].value

print(data)
print('This is a test, python3 works!')
print('<a href="../control.html">back</a>')