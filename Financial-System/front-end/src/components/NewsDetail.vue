<script setup>
import {useRoute} from "vue-router";
import {computed, onMounted, reactive} from "vue";
import logo from '@/assets/logo.png'
import {Clock} from "@element-plus/icons-vue";
import {post} from "@/net/index.js";

const route = useRoute()
const form = reactive({
  article: {
    id: route.params.id,
    photo_url: '',
    title: '',
    time: '',
    content: '',
  }
})

const relContent = computed(() => form.article.content.replace(/######/g, '<br/><br/>'))

function getArticleDetail() {
  post('/News/GetNews', {
    id: form.article.id
  }, (data) => {
    form.article = data
  })
}

onMounted(() => {
  getArticleDetail()
})
</script>



<template>
<div class="news-detail">
  <div class="headLine">
    <img :src="logo" alt="logo" style="width: 100px;height: auto"/>
  </div>
  <div class="ctt">
    <div class="left"></div>
    <div class="center">
      <div style="text-align: center;margin-top: 30px;font-size: 25px;margin-bottom: 10px;font-weight: bolder">{{form.article.title}}</div>
      <div style="display: flex;flex-direction: row;align-items: center;justify-content: center;color: grey">
        <el-icon><Clock /></el-icon>
        <span style="margin-left: 5px">{{form.article.time}}</span>
      </div>
      <div style="text-align: center;margin-top: 30px;margin-bottom: 30px">
        <img :src="form.article.photo_url" alt="配图" style="width: 600px;height: auto;"/>
      </div>
      <div style="font-size: 18px">
        <div v-html="relContent"></div>
      </div>
    </div>
    <div class="right"></div>
  </div>
</div>
</template>

<style scoped>
.news-detail{
  height: 100vh;
  width: 100%;
}

.headLine{
  height: 10vh;
  background-color: #191a1f;
  display: flex;
  align-items: center;
  justify-content: center;
}

.ctt{
  height: 90vh;
  display: flex;
  flex-direction: row;
  background-image: linear-gradient(to right bottom, #f3dbe8, #efdbec, #eadbf0, #e4dcf4, #ddddf7, #d7e0fa, #d1e3fc, #cbe6fd, #c6ebfe, #c4f0fd, #c3f5fa, #c6f9f6);
}

.right {
  width: 300px;
}
.left{
  width: 300px;
}

.center{
  background-color: white;
  flex: 1;
  overflow-y: auto;
  max-height: 90vh;
  padding: 40px 40px 40px 40px;
}
</style>