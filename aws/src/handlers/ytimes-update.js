const AWS = require('aws-sdk');

const axios = require('axios');
const chromium = require('chrome-aws-lambda');

const s3 = new AWS.S3();
const bucketName = process.env.BUCKET_NAME;
const region = process.env.AWS_REGION;

const lat = process.env.CLIENT_LAT;
const lon = process.env.CLIENT_LON;

async function getScreenShot(lat, lon) {
  let browser = null;

  browser = await chromium.puppeteer.launch({
    args: chromium.args,
    defaultViewport: chromium.defaultViewport,
    executablePath: await chromium.executablePath,
    headless: chromium.headless,
    ignoreHTTPSErrors: true,
  });

  const page = await browser.newPage();
  await page.setViewport({
    width: 540,
    height: 960,
  });

  await page.goto(
    `http://${bucketName}.s3-website-${region}.amazonaws.com/?lat=${lat}&lon=${lon}`, {
      waitUntil: 'networkidle2'
    }
  );

  const buffer = await page.screenshot({type: 'jpeg'});
  await s3.putObject({
    Body: buffer,
    Bucket: bucketName,
    Key: 'index.jpg',
    ContentType: 'image/jpeg',
    ACL: "public-read",
  }).promise();

  if (browser !== null) {
    await browser.close();
  }
}

async function getXKCD() {
  const r = await axios.get('https://xkcd.com/info.0.json');
  r.data.img = r.data.img.replace('.png', '_2x.png');
  return r.data;
}

async function getHackerNews() {
  const r = await axios.get(
    'https://hacker-news.firebaseio.com/v0/topstories.json');
  return await Promise.all(r.data.slice(0, 10).map(async (sid) => {
    const e = await axios.get(
      `https://hacker-news.firebaseio.com/v0/item/${sid}.json`);
    return e.data.title;
  }));
}

async function getArXiv() {
  const regexp = RegExp('var papers = (.*);\n', 'g');
  const r = await axios.get('https://arxiv-sanity-lite.com/');
  return JSON.parse(regexp.exec(r.data)[1]).map((e) => (
    e.title.replace('\n ', '')
  ));
}

async function getUpdate() {
  const data = {
    xkcd: await getXKCD(),
    arxiv: await getArXiv(),
    hackernews: await getHackerNews(),
    timestamp: Date.now(),
  };

  await s3.putObject({
    Body: JSON.stringify(data, null, 2),
    Bucket: bucketName,
    Key: 'json/data.json',
    ContentType: 'application/json',
    ACL: "public-read",
  }).promise();

  return data;
}

exports.yTimesUpdateHandler = async (event, context) => {
  await getUpdate();
  await getScreenShot(lat, lon);
};
