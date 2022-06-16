<template>
  <div class="home">
    <Main/>
  </div>
</template>

<script>
// @ is an alias to /src
import Main from '@/components/Main.vue';

export default {
  name: 'Home',
  components: {
    Main,
  },
};
</script>
