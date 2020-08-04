/* @file   testebbchar.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief  A Linux user space program that communicates with the ebbchar.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/ebbchar.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
 
#define BUFFER_LENGTH 32               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 


int main(){
   int ret, fd,cant_lectura;

   printf("Abriendo el dispositivo.\n");
   fd = open("/dev/i2c_gonza", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      printf("Failed to open the device...");
      return -1;
   }
   
   printf("Bytes a leer desde el dispositivo:");
   scanf("%d",&cant_lectura);
 
   if (cant_lectura > BUFFER_LENGTH)
      cant_lectura = BUFFER_LENGTH;

   printf("se leeran %d bytes\n",cant_lectura);
   printf("Reading from the device...\n");
   ret = read(fd, receive, cant_lectura);        // Read the response from the LKM
   if (ret < 0){
      printf("Failed to read the message from the device.");
      close(fd);
      return -1;
   }
   printf("The received message is: [%s]\n", receive);
   printf("End of the program\n");
   close(fd);
   return 0;
}

