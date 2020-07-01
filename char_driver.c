// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

/*Se define un major number arbitrario (estoy inventando) */
#define MY_MAJOR_NUM 202

/* Definicion de estructura cdev que representa internamente un chardevice  */
static struct cdev my_dev;

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
    int ret;

	/* Obtenemos el primer identificador de dispositivo */
	dev_t dev = MKDEV(MY_MAJOR_NUM, 0);
	
	pr_info("Char driver init\n");

	/* Asignado de device numbers */

	ret = register_chrdev_region(dev, 1, "my_char_device");

	if (ret < 0){
		pr_info("Imposible asignar major number %d\n", MY_MAJOR_NUM);
		return ret;
	}

	/* Se inicializa la estructura cdev y se la agrega al espacio kernel */
	cdev_init(&my_dev, &my_dev_fops);
	ret= cdev_add(&my_dev, dev, 1);

	if (ret < 0){
		unregister_chrdev_region(dev, 1);
		pr_info("Unable to add cdev\n");
		return ret;
	}
    return 0;
}

/**********************************************************************
 *  Funcion exit. Se ejecuta cuando el modulo es removido
 **********************************************************************/
static void __exit chardrv_exit(void) {
    pr_info("Fin del mundo\n");
    cdev_del(&my_dev);
    unregister_chrdev_region(MKDEV(MY_MAJOR_NUM, 0), 1);
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
