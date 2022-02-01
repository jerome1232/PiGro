#!/usr/bin/env python3
# -*- coding: UTF-8 -*-# enable debugging

import cgitb, cgi
import sock_client


def main():
    cgitb.enable()

    client = sock_client.SockClient(location='sock')

    print("Content-Type: text/html;charset=utf-8")
    print()
    print("<html>")
    print("<title>Threshold data submitted</title>")
    print('<link type="text/css" rel="stylesheet" href="../default.css"/>')
    print('<link type="text/css" rel="stylesheet" href="cgi.css"/>')

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
        elif key == 'sleep_time':
            item = int(item) * 60_000

        data[key] = item

    to_write = ''
    for key in data:
        to_write += str(key)
        to_write += ':'
        to_write += str(data[key])
        to_write += ','

    client.write(to_write)

    print('<main>')
    print('Data sent. It will be picked up on next sensor check.<br/>')
    print('<a href="../control.html">back</a>')
    print('</main>')


if __name__ == "__main__":
    main()