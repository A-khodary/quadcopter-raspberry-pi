# PROJET DRONE M1 INFORMATIQUE

#########################################################################
#							Compilation									#

#------------------------------LE CLIENT--------------------------------#

necessite :SDL 2 - la lib pour utiliser la manette XBOX360 (libSDL)

	make client

Pour calibrer la manette XBOX360   ->   jstest-gtk
https://apps.ubuntu.com/cat/applications/jstest-gtk/

Pour simuler manette XBOX360 from DualShocks
I) activé bluetooth 
II) bouton playstation + share -> lumiere qui clignote
III) taper "ds4drv --emulate-xboxdrv" dans le terminal


#------------------------------LE DRONE--------------------------------#

	make drone

#########################################################################


#########################################################################
#							Execution									#

pour executer le code du drone :

	./droneMain

pour executer le code du client/controlleur :

	./client 127.0.0.1


--------------
A faire

- Passer en UDP -> 99 %

- choisir une action quand il n'y a plus de manette 