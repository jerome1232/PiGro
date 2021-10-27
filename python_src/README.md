# comunication.py setup

Simply copy pigro.service to /lib/systemd/system/
and adjust it's permissions to 644

```sh
sudo cp ~/PiGro/python_src/pigro.service /lib/systemd/system/
sudo chmod 644 /lib/systemd/system/pigro.service

# reload systemctl
sudo systemctl daemon-reload

# enable pigro.service and start it
sudo systemctl enable pigro.service
sudo systemctl start pigro.service
```

Now pigro.service will automatically start at boot and can be controlled
with the systemctl command!
