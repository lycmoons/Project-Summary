<script setup>
import { nextTick, onMounted, reactive, ref, watch } from 'vue'
import { ArrowRightBold, Clock, MoreFilled } from "@element-plus/icons-vue";
import router from "@/router/index.js";
import { get } from "@/net/index.js";

const form = reactive({
  navItems: [
    '机器人概念', '低空经济', '鸿蒙概念', '稀土永磁', '智能医疗',
    '氢能源', '核电核能', '无人驾驶', '光伏', '人工智能概念',
    '北斗导航', '算力芯片', 'AI智能体', '无人机', 'LED概念',
    '融资融券'
  ],
  articles: [],
  // headLines: [],
  loading: true
})

function getArticleInfo() {
  get('/News/GetAllNews', (data) => {
    console.log(data.news);

    form.articles = data.news
    form.loading = false
  })
}

onMounted(() => {
  getArticleInfo()
})

const topicRefs = ref([])
const selectedIndex = ref(0)
watch(selectedIndex, async (newIndex) => {
  await nextTick()
  const el = topicRefs.value[newIndex]
  if (el) {
    el.scrollIntoView({
      behavior: 'smooth',
      block: 'start'
    })
  }
})

function checkNewsDetail(id) {
  router.push({
    name: 'news-content',
    params: {
      id: id
    }
  })
}

</script>


<template>
  <div class="news">
    <div class="nav-container">
      <div v-for="(item, index) in form.navItems" :key="index" class="nav-item"
        :class="{ active: selectedIndex === index }" @click="selectedIndex = index">
        {{ item }}
      </div>
    </div>

    <div class="cont">
      <div v-for="(topic, index) in form.navItems" :key="index" :ref="el => topicRefs[index] = el" class="each-topic">

        <div class="headLine">
          <h1 style="margin-right: 10px">{{ topic }}</h1>
          <el-icon style="font-size: 32px">
            <ArrowRightBold />
          </el-icon>
        </div>

        <div class="loading">
          <svg v-if="form.loading" class="loading rect" width="50" height="50" viewbox="0 0 50 50">
            <polygon points="0 0 0 50 50 50 50 0" class="polygon" />
          </svg>
        </div>

        <div v-if="!form.loading" class="articles">
          <div v-for="article in form.articles">
            <div @click="checkNewsDetail(article.id)" v-if="article.type === topic" class="article">
              <img v-if="article.photo_url !== ''" class="article-img" :src="article.photo_url" alt="photo" />
              <div style="display: flex;flex-direction: column">
                <h2>{{ article.title }}</h2>
                <div style="display: flex;flex-direction: row;color: grey;align-items: center">
                  <el-icon>
                    <Clock />
                  </el-icon>
                  <span style="margin-left: 5px">{{ article.time }}</span>
                </div>
              </div>
            </div>
          </div>
        </div>

        <hr style="margin-bottom: 30px" />

      </div>
    </div>

    <!--    <div class="other">-->
    <!--      <div class="some-button">-->
    <!--        <div class="feedback">-->
    <!--          <span style="font-size: 18px;font-weight: 900">Give Us Feedback</span>-->
    <!--        </div>-->
    <!--        <div class="addTo">-->
    <!--          <span style="font-size: 18px;font-weight: 900">Add To My Shortcuts</span>-->
    <!--        </div>-->
    <!--        <div class="getMore">-->
    <!--          <el-icon><MoreFilled /></el-icon>-->
    <!--        </div>-->
    <!--      </div>-->
    <!--      <div class="headText" style="margin-bottom: 30px">-->
    <!--        <h1>Dow Jones Top HeadLines</h1>-->
    <!--      </div>-->
    <!--      <div class="loading">-->
    <!--        <svg v-if="form.loading" class="loading rect" width="50" height="50" viewbox="0 0 50 50">-->
    <!--          <polygon points="0 0 0 50 50 50 50 0" class="polygon" />-->
    <!--        </svg>-->
    <!--      </div>-->
    <!--      <div v-if="!form.loading" class="ctt">-->
    <!--        <div v-for="item in form.headLines">-->
    <!--          <div class="ctt-item">-->
    <!--            <div style="display: flex;flex-direction: column">-->
    <!--              <h3>{{item.content}}</h3>-->
    <!--              <div style="display: flex;flex-direction: row;color: grey;align-items: center">-->
    <!--                <el-icon><Clock /></el-icon>-->
    <!--                <span style="margin-left: 5px">{{item.time}} · {{item.description}}</span>-->
    <!--              </div>-->
    <!--            </div>-->
    <!--          </div>-->
    <!--          <hr style="margin-top: 20px;margin-bottom: 20px"/>-->
    <!--        </div>-->
    <!--      </div>-->
    <!--    </div>-->
  </div>
</template>



<style scoped>
.rect .polygon {
  fill: none;
  stroke-width: 8px;
  stroke: black;
  stroke-dasharray: 50;
  stroke-dashoffset: 50%;
  animation: rect 1.3s forwards infinite;
}

@keyframes rect {
  to {
    stroke-dashoffset: 250%;
  }
}

.some-button {
  display: flex;
  flex-direction: row;
  justify-content: space-evenly;
  margin-top: 30px;
}

.article {
  display: flex;
  flex-direction: row;
  align-items: center;
  margin-bottom: 30px;
  transition: all 0.3s ease-in;
}

.ctt-item {
  transition: all 0.3s ease-in;
}

.ctt-item:hover {
  border-radius: 20px;
  background: #f5f5f5;
  box-shadow: inset 20px 20px 60px #d0d0d0,
    inset -20px -20px 60px #ffffff;
}

.ctt-item:hover h3 {
  color: blue;
}

.article-img {
  width: 150px;
  height: 110px;
  margin-right: 20px;
  margin-left: 20px;
}

.headLine {
  display: flex;
  flex-direction: row;
  align-items: center;
  margin-bottom: 30px;
}

.news {
  display: flex;
  flex-direction: row;
  height: 92vh;
}

.cont {
  flex: 1;
  overflow-y: auto;
  max-height: 100%;
  cursor: pointer;
}

.nav-container {
  display: flex;
  flex-direction: column;
  margin-left: 60px;
  margin-top: 30px;
  margin-right: 30px;
  overflow-y: auto;
  max-height: 100%;
  cursor: pointer;
}

.nav-item {
  position: relative;
  padding: 12px 16px 12px 24px;
  cursor: pointer;
  color: black;
  transition: all 0.5s ease;
  font-size: 18px;
  margin-bottom: 5px;
}

.nav-item::before {
  content: '';
  position: absolute;
  left: 0;
  top: 0;
  height: 100%;
  width: 4px;
  background-color: grey;
  transition: background-color 0.5s ease;
}

.nav-item.active {
  font-weight: bold;
  color: #1976d2;
  background-color: #f5f5f5;
}

.nav-item.active::before {
  background-color: #1976d2;
  width: 5px;
}

.nav-item:hover {
  background-color: #f0f0f0;
}

.article:hover {
  border-radius: 20px;
  background: #f5f5f5;
  box-shadow: inset 20px 20px 60px #d0d0d0,
    inset -20px -20px 60px #ffffff;
}

.article:hover h2 {
  color: blue;
  font-size: 25px;
}

h2 {
  transition: all 0.3s ease-in;
}

h3 {
  transition: all 0.3s ease-in;
}

.other {
  width: 500px;
  margin-left: 50px;
  overflow-y: auto;
  max-height: 100%;
}

.feedback {
  height: 40px;
  width: 180px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 35px;
  background: #f5f5f5;
  box-shadow: inset 20px 20px 60px #d0d0d0,
    inset -20px -20px 60px #ffffff;
  cursor: pointer;
  transition: all 0.3s ease-in;
}

.addTo {
  height: 40px;
  width: 200px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 35px;
  background: #f5f5f5;
  box-shadow: inset 20px 20px 60px #d0d0d0,
    inset -20px -20px 60px #ffffff;
  cursor: pointer;
  transition: all 0.3s ease-in;
}

.getMore {
  height: 40px;
  width: 35px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 35px;
  background: #f5f5f5;
  box-shadow: inset 20px 20px 60px #d0d0d0,
    inset -20px -20px 60px #ffffff;
  cursor: pointer;
  transition: all 0.3s ease-in;
}

.feedback:hover {
  border-radius: 35px;
  background: #c9c9c9;
  box-shadow: inset 20px 20px 60px #ababab,
    inset -20px -20px 60px #e7e7e7;
}

.addTo:hover {
  border-radius: 35px;
  background: #c9c9c9;
  box-shadow: inset 20px 20px 60px #ababab,
    inset -20px -20px 60px #e7e7e7;
}

.getMore:hover {
  border-radius: 35px;
  background: #c9c9c9;
  box-shadow: inset 20px 20px 60px #ababab,
    inset -20px -20px 60px #e7e7e7;
}
</style>
