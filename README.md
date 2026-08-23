# Eon Firmware

An ESP32-S3 firmware application that sends component telemetry data to an API endpoint, built entirely in C.

## What It Does

Simply put, it fetches telemetry data from all components it can access, packages them nicely in temporary storage and:
- if there's a Wi-Fi connection, send the data to an API endpoint,
- if there's no Wi-Fi connection, keep the data stored and continue fetching data.

When it regains connection, it flushes the backlog to the API too.
If the network is down for too long, the oldest data gets wiped and replaced with new data.

## Hardware

This is specifically meant for the main controller. In this case, the `ESP32-S3 development board`. 

## Notes

This is a personal project in early development. The code reflects that: expect rough edges.

## License

[PolyForm Noncommercial License 1.0.0](LICENSE). No commercial use.
