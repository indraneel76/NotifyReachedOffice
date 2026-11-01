import boto3
import json

# Replace 'YOUR_TOPIC_NAME' with your AWS IoT Core topic name
iot_topic = 'notifyReachedHomeTopic/sub'

def lambda_handler(event, context):
    # Create IoT client
    iot_client = boto3.client('iot-data')

    print("Received Event:")
    print(json.dumps(event))
    params = event.get("queryStringParameters") or {}

    office = params.get("office")
    thing = params.get("thing")

    print("office:", office)
    print("thing:", thing)



    # Message payload to be sent to IoT Core
    message = {
    "office":office,
    "thing":thing
    }

    # Publish message to IoT Core topic
    response = iot_client.publish(
        topic=iot_topic,
        qos=1,
        payload=json.dumps(message)
    )

    print("Message published to topic:", iot_topic)
    print("Response:", response)

    return {
        'statusCode': 200,
        'body': json.dumps('Message sent to IoT Core')
    }
