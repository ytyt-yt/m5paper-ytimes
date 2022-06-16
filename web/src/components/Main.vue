<template>
  <div class="main">
    <div class="main-flex">

      <div class="header-div">
        <div class="header-wrap">
          <div class="header-left">
            <span v-if="weather">
              <i :class="weatherIcon"></i>
              {{ Math.round(weather.current.temp) }}°C
            </span>
          </div>
          <div class="header-title">YTimes</div>
          <div class="header-right">
            <span v-if="weather">
              {{ Math.round(weather.daily[0].temp.min) }}°C /
              {{ Math.round(weather.daily[0].temp.max) }}°C
            </span>
          </div>
        </div>
        <div class="header-date">
          {{ date }}
        </div>
      </div>

      <div class="xkcd-div">
        <img v-if="news" :src="news.xkcd.img">
      </div>

      <div class="hn-div">
        <div class="sub-title">- HN -</div>
        <ol v-if="news">
          <li v-for="(s, sid) in news.hackernews" :key="sid">{{ s }}</li>
        </ol>
      </div>

      <div class="arxiv-div">
        <div class="sub-title">- arXiv - </div>
        <ol v-if="news">
          <li v-for="(s, sid) in news.arxiv" :key="sid">{{ s }}</li>
        </ol>
      </div>

      <div class="misc-div">
        Updated on {{ time }}
      </div>

      <div class="footer-div">
        <span>
          <i class="feather icon-thermometer"></i>--°C
          <i class="feather icon-droplet"></i>--%
        </span>
        <span>
          --%
          <i class="feather icon-battery"></i>
        </span>
      </div>

    </div>
  </div>
</template>

<script>
import { useRoute } from 'vue-router';

const axios = require('axios');

export default {
  name: 'Main',
  data() {
    return {
      lat: null,
      lon: null,
      weather: null,
      weatherIcon: null,
      date: null,
      time: null,
      xkcdUrl: null,
      news: null,
    };
  },
  methods: {
    async updateWeather() {
      const resp = await axios.get(
        'https://api.openweathermap.org/data/2.5/onecall', {
          params: {
            lat: this.lat,
            lon: this.lon,
            exclude: 'exclude=hourly,minutely',
            units: 'metric',
            appid: process.env.VUE_APP_OWM_APPID,
          },
        },
      );
      this.weather = resp.data;
      this.weatherIcon = `wi wi-owm-${this.weather.current.weather[0].id}`;

      this.timezone = this.weather.timezone ?? Intl.DateTimeFormat().resolvedOptions().timeZone;

      this.date = (new Date()).toLocaleDateString(
        'en-US', {
          weekday: 'long', year: 'numeric', month: 'long', day: 'numeric', timeZone: this.timezone,
        },
      );
      this.time = (new Date()).toLocaleTimeString('en-US', { timeZone: this.timezone });
    },
    async updateNews() {
      const resp = await axios.get('/json/data.json');
      this.news = resp.data;
    },
  },
  mounted() {
    const route = useRoute();
    this.lat = route.query.lat ?? process.env.VUE_APP_LAT;
    this.lon = route.query.lon ?? process.env.VUE_APP_LON;

    this.updateWeather();
    this.updateNews();
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
  .main {
    width: 540px;
    height: 960px;
    box-sizing: border-box;
    border: 1px solid black;
    padding: 10px;

    font-family: adobe-garamond-pro, serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    font-size: 28px;
  }
  .main-flex {
    width: 100%;
    height: 100%;
    display: flex;
    flex-direction: column;
    justify-content: space-between;
  }
  .header-wrap {
    display: flex;
    justify-content: space-around;
    align-items: center;
  }
  .header-left,
  .header-right {
    width: 120px;
    height: 40px;
    box-sizing: border-box;
    border: 2px solid black;
    padding: 8px 0;
    font-size: 20px;
    text-align: center;
  }
  .header-title {
    width: 200px;
    font-family: carol-gothic, serif;
    font-size: 56px;
    text-align: center;
  }
  .header-date {
    box-sizing: border-box;
    margin-top: 8px;
    padding: 2px 0;
    font-size: 16px;
    text-align: center;
    border-top: 4px solid black;
    border-bottom: 2px solid black;
  }
  .sub-title {
    font-weight: bold;
    text-align: center;
  }
  .xkcd-div {
    max-width: 100%;
    max-height: 240px;
    box-sizing: border-box;
    margin: 0 auto;
  }
  .xkcd-div img {
    max-width: 100%;
    max-height: 100%;
    object-position: center;
    object-fit: contain;
  }
  .hn-div,
  .arxiv-div {
    max-height: 226px;
    overflow-y: hidden;
  }
  ol {
    margin: 0;
  }
  .misc-div {
    box-sizing: border-box;
    padding: 10px;
    border: 2px solid black;
    font-size: 20px;
    text-align: center;
  }
  .footer-div {
    height: 24px;
    width: 100%;
    display: flex;
    font-family: 'Iosevka Term SS15', sans-serif;
    font-size: 24px;
    justify-content: space-between;
  }
  .feather {
    font-size: 24px;
  }
</style>
