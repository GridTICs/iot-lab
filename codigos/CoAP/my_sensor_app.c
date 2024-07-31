#include "contiki.h"
#include "lib/sensors.h"
#include "sht21.h"
#include <stdio.h>

PROCESS(my_sensor_app_process, "My Sensor App Process");
AUTOSTART_PROCESSES(&my_sensor_app_process);

PROCESS_THREAD(my_sensor_app_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  printf("Initializing SHT21 sensor...\n");
  SENSORS_ACTIVATE(sht21);
  while(1){
  // Espera durante 10 segundos
  	etimer_set(&timer, CLOCK_SECOND * 3);
  	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

  // Lee valores del sensor
  	int temperature = sht21.value(SHT21_READ_TEMP);
  	int humidity = sht21.value(SHT21_READ_RHUM);

  	printf("Temperature: %d.%02d degrees Celsius\n", temperature/100, temperature % 100);
  	printf("Humidity: %d.%02d percent\n", humidity/100, humidity % 100);
  }
  // Desactiva el sensor
  
  //SENSORS_DEACTIVATE(sht21);

  PROCESS_END();
}
