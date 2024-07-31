#!/usr/bin/env python3

# This file is part of the Python aiocoap library project.
#
# Copyright (c) 2012-2014 Maciej Wasilak <http://sixpinetrees.blogspot.com/>,
#               2013-2014 Christian Amsüss <c.amsuess@energyharvesting.at>
#
# aiocoap is free software, this file is published under the MIT license as
# described in the accompanying LICENSE file.

"""This is a usage example of aiocoap that demonstrates how to implement a
simple client. See the "Usage Examples" section in the aiocoap documentation
for some more information."""

import logging
import asyncio
import influxdb_client
from aiocoap import *
from influxdb import InfluxDBClient

logging.basicConfig(level=logging.INFO)

def write_to_influxdb(data):
    temperatura = data
    client = InfluxDBClient(host="192.168.1.37",port=8086, username="mariano",password="mariano",database="moteDB")
    
    #Formateo los datos para influxDB
    json_body = [
     {
        "measurement": "temp",
        "fields": {
            "temperatura": temperatura,
         }
     }
    ]
    
    client.write_points(json_body)

async def main():
    protocol = await Context.create_client_context()

    request = Message(code=GET, uri='coap://[fd00::212:4b00:430:5314]/sensor/temperatura')

    try:
        response = await protocol.request(request).response
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        print('Result: %s\n%r'%(response.code, response.payload))
        temp = (response.payload.decode('utf-8'))
        print(temp)
        write_to_influxdb(temp)

if __name__ == "__main__":
    asyncio.run(main())
