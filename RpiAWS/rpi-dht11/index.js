const AWS = require('aws-sdk');
const dynamoDB = new AWS.DynamoDB.DocumentClient();

module.exports.handler = async (event) => {
  const collection = 'RpiDHT11';

  const params = {
    TableName: collection,
    Item: {
      id: event.id,
      temperature: event.temperature,
      humidity: event.humidity,
      device: event.device,
      time_stamp: event.time_stamp
    }
  };

  try {
    await dynamoDB.put(params).promise();
    console.log('Data saved:', JSON.stringify(params, null, 2));
    return {
      statusCode: 200,
      body: JSON.stringify(
        {
          message: 'Item created in DB',
          input: event
        },
        null,
        2
      )
    };
  } catch (err) {
    console.error(err);
    return {
      statusCode: 500, // Changed to a standard HTTP error code
      body: JSON.stringify(
        {
          error: 'Error saving the item',
          input: event
        },
        null,
        2
      )
    };
  }
};
