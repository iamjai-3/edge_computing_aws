# ESP8266 AWS IoT Example

This is an example code for connecting an ESP8266 to AWS IoT using the MQTT protocol. It reads temperature and humidity data from a DHT11 sensor and publishes it to an AWS IoT topic. The code is designed to work on an ESP8266 microcontroller.

## Table of Contents

- [ESP8266 AWS IoT Example](#esp8266-aws-iot-example)
  - [Table of Contents](#table-of-contents)
  - [Prerequisites](#prerequisites)
  - [Getting Started](#getting-started)
  - [AWS IoT Setup](#aws-iot-setup)
  - [Code Overview](#code-overview)
  - [Customization](#customization)
  - [Usage](#usage)

## Prerequisites

Before using this code, make sure you have the following:

- An ESP8266 microcontroller (e.g., NodeMCU).
- The DHT11 sensor connected to pin 0.
- The Arduino IDE installed with the ESP8266 board support.
- A Wi-Fi network with the SSID and password.
- AWS IoT credentials and endpoint.
- Arduino libraries: DHT, ESP8266WiFi, PubSubClient, ArduinoJson, and BearSSL.

## Getting Started

1. Clone or download this repository to your local machine.
2. Open the Arduino IDE and install the required libraries mentioned in the code.
3. Replace the following placeholders in the code with your credentials:
   - `ssid` and `password` with your Wi-Fi network credentials.
   - `awsEndpoint` with your AWS IoT endpoint.
   - `certificatePemCrt` with your device certificate.
   - `privatePemKey` with your device private key.
   - `caPemCrt` with the AWS IoT CA certificate.
4. Upload the code to your ESP8266.

## AWS IoT Setup

To use this code with AWS IoT, you need to set up the AWS IoT service:

1. Create an AWS IoT Thing in the AWS IoT Console.
2. Attach the device certificate and private key to the Thing.
3. Make note of your AWS IoT endpoint.
4. Optionally, create an IoT topic (e.g., "edge/temp") for publishing data.

## Code Overview

- `setup()`: Initializes the DHT sensor, connects to Wi-Fi, sets the current time using SNTP, and configures the MQTT client with certificates.
- `loop()`: Reads temperature and humidity data from the DHT sensor and publishes it to the specified MQTT topic at regular intervals.
- `msgReceived()`: Callback function for handling incoming MQTT messages.
- `pubSubCheckConnect()`: Checks if the MQTT client is connected, connects if not, and subscribes to a topic.
- `setCurrentTime()`: Sets the device's time using NTP.

## Customization

You can customize this code by:

- Changing the DHT sensor type (`DHTTYPE`) and pin (`dht_dpin`) if using a different sensor or pin.
- Modifying the MQTT topic (`topic`) where data is published.
- Adjusting the data publishing interval (`lastPublish`).
- Customizing the JSON structure for the data to be published.

## Usage

1. Upload the code to your ESP8266.
2. Open the Serial Monitor to view the device's status and MQTT messages.
3. The ESP8266 will continuously publish temperature and humidity data to AWS IoT.
