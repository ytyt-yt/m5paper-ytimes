# M5Paper YTimes

E-Ink newspaper on M5Paper

<img src="https://user-images.githubusercontent.com/20288519/173987835-26802f21-4564-40ff-82b6-fc4c7d1589a0.jpg" width="300">


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
* `UpdateSchedule`: the schedule to update data, in [AWS CloundWatch Events Schedule Expression](https://docs.aws.amazon.com/AmazonCloudWatch/latest/events/ScheduledEvents.html) format. Default: Every 30min.
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
