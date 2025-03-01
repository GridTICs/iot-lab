#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#define SERVER_EP "coap://[fe80::212:4b00:430:5381]"
#define TEMP_SENSOR_URL "sensor/temperatura"
#define HUM_SENSOR_URL "sensor/humedad"

PROCESS(er_example_client, "Erbium Example Client");
AUTOSTART_PROCESSES(&er_example_client);

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1];  // Arreglo usado para crear el mensaje CoAP
static int current_request = 0;    // 0: temperatura, 1: humedad

void client_chunk_handler(coap_message_t *response) {
    const uint8_t *chunk;
  
    if (response == NULL) {
        puts("Request timed out");
        return;
    }

    int len = coap_get_payload(response, &chunk);
    printf("Response: |%.*s|\n", len, (char *)chunk);
}

PROCESS_THREAD(er_example_client, ev, data)
{
    PROCESS_BEGIN();

    // Parseamos la dirección del servidor
    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    etimer_set(&et, CLOCK_SECOND * 5);  // Temporizador para cada solicitud

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&et)) {
            printf("-- Sending GET request --\n");

            // Preparamos la solicitud GET
            coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);  // Tipo confirmable, método GET

            // Alternamos entre temperatura y humedad
            if (current_request == 0) {
                coap_set_header_uri_path(request, TEMP_SENSOR_URL);
                printf("Requesting Temperature...\n");
            } else {
                coap_set_header_uri_path(request, HUM_SENSOR_URL);
                printf("Requesting Humidity...\n");
            }

            // Enviamos la solicitud GET y esperamos respuesta
            COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);

            // Alternamos la solicitud para la próxima iteración
            current_request = (current_request + 1) % 2;

            // Reiniciamos el temporizador
            etimer_reset(&et);
        }
    }

    PROCESS_END();
}
