# M5Paper YTimes

E-Ink newspaper on [M5Paper ESP32 Kit](https://shop.m5stack.com/search?type=product&q=m5paper)

<img src="https://user-images.githubusercontent.com/20288519/173987835-26802f21-4564-40ff-82b6-fc4c7d1589a0.jpg" width="300">


## Features

* Temperature/Humidity data collected by the device.
* Weather/XKCD/HackerNews/arXiv data from the internet.
* Low power consumption: last ~3 weeks (update every 30mins).
* NTP time sync, partial refresh.


## Pipeline

* Frontend: Vue App deployed on AWS S3.
* Data Update: AWS CloundWatch periodically calls AWS Lambda function to update the json data on S3.
* Image Render: Puppeteer caputures screenshot of the frontend, and save it to S3.
* Display: Device fetches the screenshot and display it with its temperature and humidity sensor data.


## Sever setup

The sever is built with AWS SAM.


### SAM Setup

Install/Config SAM: [Getting started with AWS SAM](https://docs.aws.amazon.com/serverless-application-model/latest/developerguide/serverless-getting-started.html)

Go to `aws` directory and install dependencies:

```
$ cd aws
$ npm install
```


### Deploy SAM

```
$ sam deploy --guided
```

Parameters:

* `AppBucketName`: the UNIQUE S3 bucket name to serve the web application.
* `UpdateSchedule`: the schedule to update data, in [AWS CloundWatch Events Schedule Expression](https://docs.aws.amazon.com/AmazonCloudWatch/latest/events/ScheduledEvents.html) format. Default: every 30min.
* `Latitude`: Client Latitude
* `Longitude`: Client Longitude

The server url will be `http://<AppBucketName>.s3-website-<AWS_REGION>.amazonaws.com`


### Build Frontend

Go to `web` directory and install dependencies:

```
$ cd web
$ npm install
$ cp .env.local.example .env.local
```

Edit `.env.local`:

* `VUE_APP_OWM_APPID`: Your [Openweather API](https://openweathermap.org/appid) key.
* `VUE_APP_LAT`: Default Latitude (can be overrided by SAM settings).
* `VUE_APP_LON`: Default Longitude (can be overrided by SAM settings).

Build Frontend:

```
$ npm build
```

Upload files under `dist` directory to S3 bucket (grant public-read access under Permissions panel when upload).


## Device Setup

Go to `arduino/ytimes` directory and copy the config:

```
$ cd arduino/ytimes
$ cp ytimes_config.example.h ytimes_config.h
```

Edit `ytimes_config.h`:

* `WIFI_SSID`: WIFI SSID
* `WIFI_PWD`: WIFI password
* `TIMEZONE_OFFSET`: UTC time offset
* `IMG_URL`: `http://<AppBucketName>.s3-website-<AWS_REGION>.amazonaws.com/index.jpg`
* `REFRESH_INTERVAL`: refresh interval, in seconds.

Upload the font file `data/iosevka-fixed-ss15-medium.ttf` via SPIFFS.

Compile & Upload code with Arduino.


## Issues

Temperature sensor data is not very accurate.
