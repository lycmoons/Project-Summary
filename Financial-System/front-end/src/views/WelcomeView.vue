<script setup>
import logo from'@/assets/logo.png'
</script>

<template>
  <div class="container">
    <div class="left"></div>
    <div class="center">
      <img class="logo" :src="logo" alt="logo" style="width: 200px;height: auto">
      <div class="inputContent">
        <router-view v-slot="{ Component }">
          <transition name="el-fade-in-linear" mode="out-in">
            <component :is="Component"/>
          </transition>
        </router-view>
      </div>
    </div>
    <div class="right"></div>
  </div>
</template>

<style scoped>
.container {
  display: flex;
  height: 100vh;
  background-image: linear-gradient(to right bottom, #482d3c, #452f42, #413246, #3d344b, #37374e, #313b52, #2c3e54, #264255, #214756, #1e4b55, #1f4f53, #255350);
}

.left, .center, .right {
  padding: 10px;
}

.left {
  flex: 2;
}

.center {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
}

.right {
  flex: 2;
}

.inputContent{
  flex: 1;
}

.logo{
  margin-top: 100px;
}
</style>