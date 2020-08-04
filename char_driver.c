// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>

/* Strings definiendo el nombre del dispositivo */
#define DEVICE_NAME 		"EK_IMD"

/* Puntero a estructura que representa un dispositivo esclavo
 * conectado al bus*/

static struct i2c_client *slaveDevice;


/*
 * Definicion de los ID correspondientes al Device Tree. Estos deben ser informados al
 * kernel mediante la macro MODULE_DEVICE_TABLE
 *
 * NOTA: Esta seccion requiere que CONFIG_OF=y en el kernel
 */

static const struct of_device_id ekIMD_dt_ids[] = {
	{ .compatible = "mse,EK_IMD", },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, ekIMD_dt_ids);



/*********************************************************************************
 * Definiciones de funciones sobre archivos y estructura correspondiente
 *********************************************************************************/


static int ekIMD_open(struct inode *inode, struct file *file)  {
	pr_info("my_dev_open() fue invocada.\n");
	return 0;
}

static int ekIMD_close(struct inode *inode, struct file *file)  {
	pr_info("my_dev_close() fue invocada.\n");
	return 0;
}

static long ekIMD_ioctl(struct file *file, unsigned int cmd, unsigned long arg)  {
	pr_info("my_dev_ioctl() fue invocada. cmd = %d, arg = %ld\n", cmd, arg);
	return 0;
}

static ssize_t ekIMD_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset)  {
	pr_info("my_dev_write() fue invocada.");
	return 0;
}

static ssize_t ekIMD_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)  {
	pr_info("my_dev_read() fue invocada.");
	return 0;
}



/* declaracion de una estructura del tipo file_operations */

static const struct file_operations my_dev_fops = {
	.owner = THIS_MODULE,
	.open = ekIMD_open,
	.read = ekIMD_read,
	.write = ekIMD_write,
	.release = ekIMD_close,
	.unlocked_ioctl = ekIMD_ioctl,
};

/*--------------------------------------------------------------------------------*/


/* Declaracion e inicializacion de una estructura miscdevice */
static struct miscdevice ekIMD_miscdevice = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = DEVICE_NAME,
		.fops = &my_dev_fops,
};




//************************************************************************************
/*
 * Definiciones de funciones probe y remove necesarias para el driver
 *
 * NOTA: No reemplazan las funciones init y exit, sino que se definen mediante
 * la macro module_i2c_driver() internamente
 */


static int ekIMD_probe(struct i2c_client *connected_slave, const struct i2c_device_id *id)  {
	int ret_val;

	pr_info("Ejecutando funcion probe()\n");
	pr_info("Informacion de dispositivo conectado (struct i2c_client):\n");
	pr_info("\tDireccion: %X\n",connected_slave->addr);
	pr_info("\tNombre: %s\n",connected_slave->name);
	pr_info("\tDriver: %s\n",connected_slave->dev->driver->name);

	pr_info("\n\nInformacion desde ID (struct i2c_device_id):\n");
	pr_info("\tNombre: %s",id->name);



	/* Registro del dispositivo con el kernel */
	ret_val = misc_register(&ekIMD_miscdevice);
	if (ret_val != 0) {
		pr_err("No se pudo registrar el dispositivo %s\n", DEVICE_NAME);
		return ret_val;
	}
	pr_info("%s: minor asignado: %i\n", DEVICE_NAME, ekIMD_miscdevice.minor);

	pr_info("Informacion de dispositivo conectado luego de registrar:\n");
	pr_info("\tClase: %s\n",connected_slave->dev->class->name);
	pr_info("\tMajor number: %d\n",MAJOR(connected_slave->dev->devt));

	/*
	 * Una vez que tenemos numeros Major y Minor, exponemos al modulo completo
	 * el puntero recibido como dispositivo esclavo conectado (cliente)
	 */
	slaveDevice = connected_slave;	//guardamos este puntero para el uso de read y write

	return 0;
}


static int ekIMD_remove(struct i2c_client *client)  {

	pr_info("Ejecutando funcion remove()\n");

	/* Unregister del miscDevice del kernel */
	misc_deregister(&ekIMD_miscdevice);

	pr_info("Modulo descargado, anulado el registro");

}



static struct i2c_driver ekIMD_driver = {

	.probe= ekIMD_probe,
	.remove= ekIMD_remove,
	.driver = {
		.name = "EK_IMD",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(ekIMD_dt_ids),
	},
};

/*----------------------------------------------------------------------*/



/**********************************************************************
 * Esta seccion define cuales funciones seran las ejecutadas al cargar o
 * remover el modulo respectivamente. Es hecho implicitamente,
 * termina declarando probe() y remove()
 **********************************************************************/
module_i2c_driver(ekIMD_driver);

/**********************************************************************
 * Seccion sobre Informacion del modulo
 **********************************************************************/
MODULE_AUTHOR("Gonzalo E. Sanchez <gonzalo.e.sds@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Este modulo es un driver para EK_IMD");
MODULE_INFO(mse_imd, "Esto no es para simples mortales");
