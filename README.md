# PROJET DRONE M1 INFORMATIQUE



### PREPARATION Raspberry Pi 2

Install RASPBIAN JESSIE Kernel version:4.4 on the Raspberry Pi 2

Active SSH , I2C

```
Patch the Raspberry witch the kernel_4.4.47_RT.tgz 

	pi@raspberry ~$ tar xzf kernel_4.4.47_RT.tgz
	pi@raspberry ~$ sudo rm -r /lib/firmware/
	pi@raspberry ~$ sudo rm -r /boot/overlays/
	pi@raspberry ~$ cd boot
	pi@raspberry ~$ sudo cp -rd * /boot/
	pi@raspberry ~$ cd ../lib
	pi@raspberry ~$ sudo cp -dr * /lib/
```

```
Disable USB RT PREEMPT LIMITATION -> add to /boot/cmdline.txt : 
	dwc_otg.fiq_enable=0
	dwc_otg.fiq_fsm_enable=0
```

```
Disable Garde-fou -> add to .profile:
	sudo sysctl kernel.sched_rt_runtime_us=-1
```

```
Frequence I2C -> sudo nano /etc/modprobe.d/i2c.conf
	options i2c_bcm2708 baudrate=400000
```



### Compilation

##### CLIENT/CONTROLLER

dependencies : SDL - for XBOX360 Controller (libSDL)

	make client

Pour calibrer la manette XBOX360   ->   jstest-gtk
https://apps.ubuntu.com/cat/applications/jstest-gtk/

Pour simuler manette XBOX360 from DualShocks
	* activé bluetooth 
	* bouton playstation + share -> lumiere qui clignote
	* taper "ds4drv --emulate-xboxdrv" dans le terminal

##### DRONE

	make drone

### Execution

for the Drone on the Raspberry Pi 2:

	sudo ./droneMain

for the Client/Controller :

	./client 127.0.0.1


--------------
TODO

- choisir une action quand il n'y a plus de manette

- Corriger probleme avec la version actuel du kernel sur raspberry , ET PREEMP

- Filtrer valeur accelerometre verticale et altimetre pour le PID d'altitude

- Catcher Signaux System UNIX