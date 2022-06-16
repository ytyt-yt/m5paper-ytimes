# YTimes

E-Ink newspaper on M5Paper


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
* `Timezone`: Client Timezone ([TZ database Name](https://en.wikipedia.org/wiki/List_of_tz_database_time_zones)).
* `Latitude`: Client Latitude
* `Longitude`: Client Longitude

The server url will be `http://<AppBucketName>.s3-website-<AWS_REGION>.amazonaws.com`

### Build Frontend
```
$ cd ../web/
$ npm install
$ npm build
```

Upload files under `dist` directory to S3 bucket (grant public-read access under Permissions panel when upload).
