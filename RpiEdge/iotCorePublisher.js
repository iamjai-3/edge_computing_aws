const awsIoT = require('aws-iot-device-sdk');
const crypto = require('crypto');
const endpointFile = require('./endpoint.json');

const deviceName = __dirname.split('\\').pop();

// Create the thingShadow object with argument data
const device = awsIoT.device({
  keyPath: 'certs/private.pem.key',
  certPath: 'certs/certificate.pem.crt',
  caPath: 'certs/root-CA.crt',
  clientId: deviceName,
  host: endpointFile.endpointAddress
});

device.on('connect', function () {
  console.log('Connected to AWS IoT');
  infiniteLoopPublish();
});

function infiniteLoopPublish() {
  console.log('Sending data to AWS IoT for ' + deviceName);
  device.publish('device/dht11', JSON.stringify(getDHT11Data(deviceName)));
  setTimeout(infiniteLoopPublish, 5000);
}

function randomFloatBetween(minValue, maxValue) {
  return parseFloat(
    Math.min(minValue + Math.random() * (maxValue - minValue), maxValue)
  );
}

// Generate random temperature data based on the deviceName -> This actually comes from sensor DHT11
function getDHT11Data(deviceName) {
  const message = {
    id: crypto.randomBytes(15).toString('hex'),
    temperature: randomFloatBetween(-20, 40), // Temperature in Celsius (-20°C to 40°C)
    humidity: randomFloatBetween(0, 100), // Humidity in percentage (0% to 100%)
    device: deviceName,
    time_stamp: new Date().toISOString().replace(/\..+/, '')
  };
  return message;
}
