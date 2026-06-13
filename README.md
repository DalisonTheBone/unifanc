# unifanc
Universal, module based fan control program for linux. Rewrite of unifan to c, works on any distro.
## Supported
- (thinkpad-universal.c): Any thinkpad with thinkpad_acpi support for hwmon or proc sensors and fan control


## Install
Dependencys: gcc
**Install Commands:**:
```sh
git clone https://github.com/DalisonTheBone/unifan.git
cd unifan
sudo bash ./install.sh
```

## Adding Modules & Configs
At the moment, no cli has been created for this project yet. However you can add a module or config via:
### Modules
Compile your desired module to /etc/unifand/module
**Example:**
```sh
sudo gcc -Wall -Wextra -o /etc/unifand/module ./modules/thinkpad-universal.c
```
### Configs
Add or edit your desired config to /etc/unifand/unifand.conf
**Example:**
```sh
sudo cp unifand.conf /etc/unifand/unifand.conf
```
### Reload Daemon
Reload the daemon to make sure changes save properly
**Systemd Example:**
```sh
sudo systemctl restart unifand.service
```
## Creating Custom Modules & Configs
Unifan is module based, meaning if a module for your system doesn't exist you can make it!

### Creating Custom Modules
Modules can be any binary at /etc/unifand/module and comunicate with the daemon and everything else via arguments.
**Argument Use Format:**
```sh
./module i
# Sets up system to be able to read tempature and set fan speed for the duration of the session.
# Shouldn't be presistant across boots since it's run at boot

./module g
# Gets tempature and returns it as a interger on one line.
# Ex: ./module g -> Output: "45"

./module s $speed_percent
# Sets the fan speed to a percent as an interger (0-100)
# Ex: ./module s 75 -> sets fan speed to 75% of the maximum
```
### Creating Custom Configs
Way easier then making a custom module, edit /etc/unifand/unifand.conf
**Example Config:**
```
cnt=4
0-45=50
60-65=64
70-75=90
85-120=100
``
**Usage:**
- First line must be cnt= followed by the number of fan rules below it, without containing a space.
- Subsequent lines are fan rules and must be one after another. formated like so: temo_min-temp_max=fan_speed_percent
  - the fan speed will be picked based on witch range the current tempature it fits into, or the range with the highest temp_max value that is lower then the current tempature.



