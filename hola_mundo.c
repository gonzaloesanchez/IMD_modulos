// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>

/* Modulo "hello world". Muestra la funcionalidad de las secciones
 * __init y __exit */

/*
 *  Funcion Init. Se ejecuta cuando el modulo se carga
 */
static int __init helloworld_init(void) {
    pr_info("Hola Mundo!\n");
    return 0;
}

/*
 *  Funcion exit. Se ejecuta cuando el modulo es removido
 */
static void __exit helloworld_exit(void) {
    pr_info("Fin del mundo\n");
}

/*
 * Esta seccion define cuales funciones seran las ejecutadas al cargar o
 * remover el modulo respectivamente
 */
module_init(helloworld_init);
module_exit(helloworld_exit);

/*
 * Seccion sobre Informacion del modulo
 */
MODULE_AUTHOR("Gonzalo E. Sanchez <gonzalo.e.sds@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_INFO(mse_imd, "Esto no es para simples mortales");
