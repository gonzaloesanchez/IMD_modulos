// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

/* Strings definiendo la clase y el nombre del dispositivo */
#define DEVICE_NAME 		"GonzaHW"
#define CLASS_NAME 			"WhatKindOf_SorceryIsThis"
#define DEV_MINOR_NUMBER	0
#define MINOR_BASE_NUMBER	0
#define DEVICE_COUNT		1


/* Definiciones globales para este archivo:
 * 	-Estructura cdev que representa internamente un chardevice
 * 	-Puntero a estructura de clase de dispositivo
 * 	-Variable tipo dev_t que contiene un numero Major y Minor
 * 	 */
static struct cdev my_dev;
static struct class* gonzaClass;
static dev_t dev;

/*********************************************************************************
 * Definiciones de funciones sobre archivos y estructura correspondiente
 *********************************************************************************/


static int my_dev_open(struct inode *inode, struct file *file)  {
	pr_info("my_dev_open() fue invocada.\n");
	return 0;
}

static int my_dev_close(struct inode *inode, struct file *file)  {
	pr_info("my_dev_close() is called.\n");
	return 0;
}

static long my_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)  {
	pr_info("my_dev_ioctl() is called. cmd = %d, arg = %ld\n", cmd, arg);
	return 0;
}

/* declaracion de una estructura del tipo file_operations */

static const struct file_operations my_dev_fops = {
	.owner = THIS_MODULE,
	.open = my_dev_open,
	.release = my_dev_close,
	.unlocked_ioctl = my_dev_ioctl,
};

/*--------------------------------------------------------------------------------*/



/**********************************************************************
 *  Funcion Init. Se ejecuta cuando el modulo se carga
 **********************************************************************/
static int __init chardrv_init(void) {
	dev_t dev_no;
	int Major, ret;
	struct device* gonzaDev;

	pr_info("Hello world init\n");

	/* Asignar dinamicamente numeros de dispositivos, solo uno en este driver */
	ret = alloc_chrdev_region(&dev_no, MINOR_BASE_NUMBER, DEVICE_COUNT, DEVICE_NAME);

	/*
	 * Assert para asegurar que el dispositivo pudo asignarse de manera correcta
	 */

	if (ret < 0){
		pr_info("Unable to allocate Mayor number \n");
		return ret;
	}

	/*
	 * En este caso obtenemos los identificadores utilizando MKDEV.
	 * Esto solo se hace para propositos didacticos, dado que se
	 * utiliza un solo identificador en este driver, y dev_no podria
	 * ser utilizado como parametro para cdev_add() y device_create()
	 * sin la necesidad de utilizar la macro MKDEV
	*/

	
	/*
	 *  Obtenemos el majorNumber del primer identificador de dispositivo
	 * (el unico en este ejemplo) y luego el primer identificador
	 * completo de dispositivo, que proviene de dev_no
	 */

	Major = MAJOR(dev_no);
	dev = MKDEV(Major,DEV_MINOR_NUMBER);
	pr_info("Allocated correctly with major number %d\n", Major);

	/* Inicializar estructura cdev y agregarla a espacio kernel. */
	cdev_init(&my_dev, &my_dev_fops);
	ret = cdev_add(&my_dev, dev, DEVICE_COUNT);

	/* Assert para verificar el correcto agregado de dispositivo al espacio kernel */
	if (ret < 0){
		unregister_chrdev_region(dev, DEVICE_COUNT);
		pr_info("Unable to add cdev\n");
		return ret;
	}

	/* Registro de la clase de dispositivo (device class) */
	gonzaClass = class_create(THIS_MODULE, CLASS_NAME);

	/* Assert para verificar la correcta creacion de clase de dispositivo
	 * Notar como en cada paso, en caso de error, se debe hacer el proceso
	 * contrario para liberar memoria */

	if (IS_ERR(gonzaClass)){
		cdev_del(&my_dev);
		unregister_chrdev_region(dev, DEVICE_COUNT);
		pr_info("Failed to register device class\n");
		return PTR_ERR(gonzaClass);
	}
	pr_info("device class registered correctly\n");



	/* Creacion de un nodo device llamado DEVICE_NAME asociado a dev */
	gonzaDev = device_create(gonzaClass, NULL, dev, NULL, DEVICE_NAME);

	/*
	 * Assert para verificar que el dispositivo se creo de forma correcta.
	 * Observar el uso de macros para transformar los valores de punteros
	 * en errores, los que son devueltos por sus correspondientes funciones
	 * de creacion
	 */
	if (IS_ERR(gonzaDev)){
		class_destroy(gonzaClass);
		cdev_del(&my_dev);
		unregister_chrdev_region(dev, 1);
		pr_info("Failed to create the device\n");
		return PTR_ERR(gonzaDev);
	}
	pr_info("The device is created correctly\n");

	return 0;
}

/**********************************************************************
 *  Funcion exit. Se ejecuta cuando el modulo es removido
 *  Toda la asignacion de memoria debe ser liberada de manera
 *  inversa a como se asigno
 **********************************************************************/
static void __exit chardrv_exit(void) {
    device_destroy(gonzaClass, dev); /* remove the device */
    class_destroy(gonzaClass); /* remove the device class */
    cdev_del(&my_dev);
    unregister_chrdev_region(dev, 1); /* unregister the device numbers */
    pr_info("Fin del mundo\n");
}

/*----------------------------------------------------------------------*/



/**********************************************************************
 * Esta seccion define cuales funciones seran las ejecutadas al cargar o
 * remover el modulo respectivamente
 **********************************************************************/
module_init(chardrv_init);
module_exit(chardrv_exit);

/**********************************************************************
 * Seccion sobre Informacion del modulo
 **********************************************************************/
MODULE_AUTHOR("Gonzalo E. Sanchez <gonzalo.e.sds@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Este modulo interactua con el system call ioctl");
MODULE_INFO(mse_imd, "Esto no es para simples mortales");
