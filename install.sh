sudo mkdir -p /etc/unifand
sudo cp ./unifan.conf /etc/unifand

sudo gcc -Wall -Wextra -o /bin/unifand ./unifand.c
sudo gcc -Wall -Wextra -o /etc/unifand/module ./thinkpad-universal.c

sudo cp ./unifand.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable --now unifand.service