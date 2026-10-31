import json
import os

import boto3

thing_name="NotifyOfficeReached"
iot_data = boto3.client(
    "iot-data",
    endpoint_url=os.environ["IOT_DATA_ENDPOINT"],
)

def lambda_handler(event, context):
    print("Received Event:")
    print(json.dumps(event))
    params = event.get("queryStringParameters") or {}

    office = params.get("office")
    thing = params.get("thing")
  

    print("office:", office)
    print("thing:", thing)



    response = iot_data.update_thing_shadow(
        thingName=thing,
        payload=json.dumps({
            "state": {
                "desired": {
                    "officestate": office,
                }
            }
        }).encode("utf-8"),
    )

    shadow = json.loads(response["payload"].read())

    return {
        "statusCode": 200,
        "body": json.dumps({
            "thing_name": thing,
            "desired": shadow["state"]["desired"],
            "version": shadow.get("version"),
        }),
    }