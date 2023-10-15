import time

import Adafruit_DHT
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

sensor = Adafruit_DHT.DHT11

# AWS IoT configuration
mqtt_client_id = "rpiEdgeId"
endpoint = ""
root_ca_path = "CA.pem"
private_key_path = "private.pem.key"
certificate_path = "certificate.pem.crt"

myMQTTClient = AWSIoTMQTTClient(mqtt_client_id)
myMQTTClient.configureEndpoint(endpoint, 8883)
myMQTTClient.configureCredentials(root_ca_path, private_key_path, certificate_path)
myMQTTClient.configureAutoReconnectBackoffTime(1, 32, 20)
myMQTTClient.configureOfflinePublishQueueing(-1)
myMQTTClient.configureDrainingFrequency(2)
myMQTTClient.configureConnectDisconnectTimeout(10)
myMQTTClient.configureMQTTOperationTimeout(5)

print("Initiating Realtime Data Transfer From Raspberry Pi...")
myMQTTClient.connect()

gpio = 17

try:
    while True:
        humidity, temperature = Adafruit_DHT.read_retry(sensor, gpio)

        if humidity is not None and temperature is not None:
            print(f"Temp={temperature:.1f}*C  Humidity={humidity:.1f}%")
            payload = (
                f'{{"temperature": "{temperature:.1f}", "humidity": "{humidity:.1f}"}}'
            )
            myMQTTClient.publish(topic="rpi/dth11", QoS=1, payload=payload)
        else:
            print("Failed to get reading. Try again!")

        time.sleep(1)

except KeyboardInterrupt:
    print("Interrupted by the user")
