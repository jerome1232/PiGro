#!/bin/sh
./compile.sh && 
	sudo systemctl stop pigro.service && 
	./upload.sh && 
	sudo systemctl start pigro.service
