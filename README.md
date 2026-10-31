# NotifyReachedOffice

## Description
The project is about creating a solution where one
The repo has about creating backend service which will integrate with IFTT and Arduino based IOT ecosystem

## Use Case
The idea was around a personal usecase. There was a need that i need to inform my family members when i reach office and leave office. It would be great if there a a IOT gadget installed at my home which would notify my family members when i reach or exit office.

## High level Design
The high level design is that there would a IFTTT mobile app (can be already installed from playstore) and uses geofencing to trigger location based action like triggering a webhook.

This webhook would be implemented as AWS lambda service (python based). AWS lambda will trigger AWS IOT Hub which will inturn send message to IOT device ESP8266(node mcu) via MQTT protocol.



```mermaid
flowchart LR
    A[IFTTT Mobile App] --> B[IFTTT Webhook]
    B --> C[Backend AWS lambda Service]
    C --> D[AWS IOT Hub]
    D --> E[ESP8266]

 ```
 ## IOT Device / Gagdet functionality
The device will have 3 Led lights and buzzer (for alarm)

- Led - Green - Reached office
- Led - Red - Left office
- Led - Blue - Reached Home


![rough image diagram](rough-device-sketch.png)

## Sequence of building the solution
1. IFTTT mobile app installed and configured for location based trigger and webhook service action 

[Android play store link](https://play.google.com/store/apps/details?id=com.ifttt.ifttt&hl=en_IN)

[Apple App Store link](https://apps.apple.com/in/app/ifttt-automate-work-and-home/id660944635)


2. Integrating Webhook lambda with AWS IOT HUB .We would also have to create AWS IOT device for the same.

- Go to IOT core service
- Click on "All devices" on the left nav
- Click on Things
- Click on create Things button
- Choose "Create single Thing"
- Give name "notifyReachedOfficeDevicce"
- Choose "Unnamed shadow" and click on next
- Select "Auto-generate a new certificate (recommended)"
- Click on "create Thing"
- Click on "download All certificate" and store in a folder

3. Build AWS lambda webhook.

- Go to aws console and choose lamba service and click on create Event function
- Select author from scratch
- Function name as "NotifyReachedOffice"
- Runtime as python
- Click on create Event function button
- Paste the content inside code from webhook-lambda-notify-reached-office.py
- Click on deploy
- Click on configuration tab
- Click on Environmental Variable , click on edit and click on add 
- Click on triggers side nav
- Click on add Trigger and select API Gateway
- API type HTTP API
- Security Open
- CLick on Add
- Note down the API Gateway URL
- update the webhook URL of IFTTT with this URL
- We need to know the IOT_DATA_ENDPOINT variable . run  aws iot describe-endpoint --endpoint-type iot:Data-ATS --region ap-south-1

- Go to configuration -> Environmental variable and set the IOT_DATA_ENDPOINT variable as https://<the IOT data end point value you have earlier got> . it would be something like https://xxxxxx-ats.iot.ap-south-1.amazonaws.com
- Add permission to lambda for accessing IOT
- Click on general configuration. Edit and add role. paste the role as shown in the file Lambda_iot_permission

4. Configure IFTTT with correct Trigger
4.1  Open the IFTTT app you have installed.
4.2 Click on Create Applet . Choose classic tab . 
4.3 click add button for "if this section"
4.4 Choose service as location. Enter any area and choose your location and click on create trigger
4.5. Under "Then that" section click add choose webhook and select make a web request .paste the API gateway URL you have earlier created and add query parameter ?office=entered
4.6. Repeat the same stuff and create another applet for exit scenario and this time change the query parameter to ?office=exit


5. Building Arduino Gadget 
5.1 Wiring and conneecting various components
5.2 Writing Arduino code for integrating Arduino gadget and AWS IOT Hub and functionality. 




