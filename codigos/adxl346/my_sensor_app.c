#include "contiki.h"
#include "lib/sensors.h"
#include "adxl346.h"
#include <stdio.h>

PROCESS(my_sensor_app_process, "My Sensor App Process");
AUTOSTART_PROCESSES(&my_sensor_app_process);

PROCESS_THREAD(my_sensor_app_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  SENSORS_ACTIVATE(adxl346);

  while (1)
  {
    int x_raw = adxl346.value(ADXL346_READ_X);
    int y_raw = adxl346.value(ADXL346_READ_Y);
    int z_raw = adxl346.value(ADXL346_READ_Z);

    int x_mg = adxl346.value(ADXL346_READ_X_mG);
    int y_mg = adxl346.value(ADXL346_READ_Y_mG);
    int z_mg = adxl346.value(ADXL346_READ_Z_mG);

    printf("X raw: %d, Y raw: %d, Z raw: %d\n", x_raw, y_raw, z_raw);
    printf("X mg: %d, Y mg: %d, Z mg: %d\n", x_mg, y_mg, z_mg);

    // Espera durante 1 segundo
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}

