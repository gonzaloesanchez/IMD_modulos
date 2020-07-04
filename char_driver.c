// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

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


/* Declaracion e inicializacion de una estructura miscdevice */
static struct miscdevice gonza_miscdevice = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "GonzaHW",
		.fops = &my_dev_fops,
};



/**********************************************************************
 *  Funcion Init. Se ejecuta cuando el modulo se carga
 **********************************************************************/
static int __init chardrv_init(void) {
	int ret_val;

	pr_info("Hello world init\n");
	
	/* Registro del dispositivo con el kernel */
	ret_val = misc_register(&gonza_miscdevice);
	if (ret_val != 0) {
		pr_err("could not register the misc device mydev");
		return ret_val;
	}
	pr_info("GonzaHW: minor asignado %i\n",gonza_miscdevice.minor);
	return 0;
}

/**********************************************************************
 *  Funcion exit. Se ejecuta cuando el modulo es removido
 *  Toda la asignacion de memoria debe ser liberada de manera
 *  inversa a como se asigno
 **********************************************************************/
static void __exit chardrv_exit(void) {
	pr_info("Fin del mundo\n");

	/* Unregister del miscDevice del kernel */
	misc_deregister(&gonza_miscdevice);
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
