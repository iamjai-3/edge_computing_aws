#include "DHT.h"
#include <time.h>
#include "sntp.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DHTTYPE DHT11 // DHT 11 SENSOR
#define dht_dpin 0

DHT dht(dht_dpin, DHTTYPE);

const char *ssid = "";
const char *password = "";

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing

const char *awsEndpoint = ""; // your IoT endpoint
const char *topic = "edge/temp";

// For the two certificate strings below paste in the text of your AWS
// device certificate and private key:

// xxxxxxxxxx-certificate.pem.crt
static const char certificatePemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

// xxxxxxxxxx-private.pem.key
static const char privatePemKey[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)EOF";

// This is the AWS IoT CA Certificate from:
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid
// until January 16, 2038 so unless it gets revoked you can leave this as is:
static const char caPemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

BearSSL::X509List client_crt(certificatePemCrt);
BearSSL::PrivateKey client_key(privatePemKey);
BearSSL::X509List rootCert(caPemCrt);

WiFiClientSecure wiFiClient;
void msgReceived(char *topic, byte *payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient);

void setup()
{
    Serial.begin(9600);
    Serial.println();
    Serial.println("ESP8266 AWS IoT Example");

    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);
    WiFi.waitForConnectResult();
    Serial.print(", WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());

    // get current time, otherwise certificates are flagged as expired
    sntp_stop(); // speeds up the connection with UDP servers
    sntp_init(); // speeds up the connection with UDP servers
    setCurrentTime();

    wiFiClient.setClientRSACert(&client_crt, &client_key);
    wiFiClient.setTrustAnchors(&rootCert);
}

unsigned long lastPublish;
int msgCount;

void loop()
{

    pubSubCheckConnect();

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.printf("Current humidity = %.2f%%  temperature = %.2fC\n", h, t);

    if (millis() - lastPublish > 10000)
    {
        StaticJsonDocument<100> jsonDocument; // Adjust the size as needed
        jsonDocument["humidity"] = h;
        jsonDocument["temperature"] = t;
        String jsonString;
        serializeJson(jsonDocument, jsonString);
        pubSubClient.publish(topic, jsonString.c_str());
        Serial.print("Published: ");
        lastPublish = millis();
    }
}

void msgReceived(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message received on ");
    Serial.print(topic);
    Serial.print(": ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void pubSubCheckConnect()
{
    if (!pubSubClient.connected())
    {
        Serial.print("PubSubClient connecting to: ");
        Serial.print(awsEndpoint);
        while (!pubSubClient.connected())
        {
            Serial.print(".");
            pubSubClient.connect("ESP-EDGE");
        }
        Serial.println(" connected");
        pubSubClient.subscribe("inTopic");
    }
    pubSubClient.loop();
}

void setCurrentTime()
{
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}