#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
int main(void)  {
	/* Para que esta aplicacion funcione hay que ejecutar el comando
	 *  "mknod /dev/mi_dispositivo c 202 0" para crear /dev/mi_dispositivo */
	
	int my_dev = open("/dev/mi_dispositivo", 0);
	if (my_dev < 0) {
		perror("Fail to open device file: /dev/mi_dispositivo.");
	} else {
		ioctl(my_dev, 100, 110); /* cmd = 100, arg = 110. */
		close(my_dev);
	}
	return 0;
}
