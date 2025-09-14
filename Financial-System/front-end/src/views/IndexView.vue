<script setup>
import {ref, watch} from "vue";
import router from "@/router/index.js";
import logo from '@/assets/logo.png'

let activeMenu = ref(sessionStorage.getItem('activeMenu') || '/index')

watch(activeMenu, (newVal) => {
  sessionStorage.setItem('activeMenu', newVal)
})

function handleSelect(index) {
  activeMenu.value = index
  router.push(index)
}
</script>



<template>
  <div class="cctt">
    <div class="nnvv">
      <div class="llgg">
        <img :src="logo" alt="logo" style="height: 75px;width: auto;"/>
      </div>
      <div class="menu">
        <el-menu :default-active="activeMenu"
                 @select="handleSelect"
                 mode="horizontal"
                 style="display: flex; justify-content: space-evenly;height: 100%"
                 background-color="#191a1f"
                 text-color="grey"
                 active-text-color="white"
                 class="custom-hover-menu"
                 router>
          <el-menu-item class="menu-item" index="/index">新闻快讯</el-menu-item>
          <el-menu-item class="menu-item" index="/index/industry">行业信息</el-menu-item>
          <el-menu-item class="menu-item" index="/index/chart">图表分析</el-menu-item>
          <el-menu-item class="menu-item" index="/index/credit">企业信用评估</el-menu-item>
          <el-menu-item class="menu-item" index="/index/risk">金融风险预测</el-menu-item>
        </el-menu>
      </div>
    </div>

    <div class="content">
      <router-view/>
    </div>
  </div>
</template>



<style scoped>
.cctt {
  display: flex;
  flex-direction: column;
  height: 100vh;
}

.nnvv {
  flex: 2;
  width: 100%;
  display: flex;
  flex-direction: row;
  background-color: #191a1f;
}

.menu{
  flex: 1;
  height: 100%;
}

.llgg {
  width: 150px;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}

.content {
  flex: 22;
  width: 100%;
}

.custom-hover-menu .el-menu-item:hover {
  color: white;
}

::v-deep(.el-menu--horizontal) {
  border-bottom: none !important;
}

</style>