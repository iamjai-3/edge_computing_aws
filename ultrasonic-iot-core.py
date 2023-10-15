import _thread
import json
import ssl
import time

import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

GPIO_TRIGGER = 11
GPIO_ECHO = 18

GPIO.setup(GPIO_TRIGGER, GPIO.OUT)
GPIO.setup(GPIO_ECHO, GPIO.IN)


def on_connect(client, userdata, flags, rc):
    print("Connected to AWS IoT: " + str(rc))


client = mqtt.Client()
client.on_connect = on_connect
client.tls_set(
    ca_certs="./HCSR04_CERTS/root-CA1.pem",
    certfile="./HCSR04_CERTS/certificate.pem.crt",
    keyfile="./HCSR04_CERTS/private.pem.key",
    tls_version=ssl.PROTOCOL_SSLv23,
)
client.tls_insecure_set(True)
client.connect("a1cphkbjhsixic-ats.iot.us-east-1.amazonaws.com", 8883, 60)


def publishData(txt):
    print(txt)

    while True:
        GPIO.output(GPIO_TRIGGER, True)

        time.sleep(0.00001)
        GPIO.output(GPIO_TRIGGER, False)

        StartTime = time.time()
        StopTime = time.time()

        while GPIO.input(GPIO_ECHO) == 0:
            StartTime = time.time()

        while GPIO.input(GPIO_ECHO) == 1:
            StopTime = time.time()

        TimeElapsed = StopTime - StartTime

        distance = (TimeElapsed * 34300) / 2

        print("Measured Distance = %.1f cm" % distance)

        client.publish(
            "hcsr04/data",
            payload=json.dumps({"distance": distance}),
            qos=0,
            retain=False,
        )

        time.sleep(5)


_thread.start_new_thread(publishData, ("Spin-up new Thread...",))

client.loop_forever()
