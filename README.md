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

