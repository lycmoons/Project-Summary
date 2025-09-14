<template>
  <div class="company">
    <div class="headLine">
      <div class="headImg">
        <img :src="logo" alt="logo" style="height: 85px; width: auto"/>
      </div>

      <div class="headTitle">
        <h1>企业详情</h1>
      </div>
    </div>
    <div class="company-detail">
      <div class="companyCard" v-if="!form.loading">
        <el-card class="card" shadow="hover">
          <div class="section-title">基础信息</div>
          <el-descriptions :column="3" border>
            <el-descriptions-item label="UID">{{ form.company.uid }}</el-descriptions-item>
            <el-descriptions-item label="户号">{{ form.company.account }}</el-descriptions-item>
            <el-descriptions-item label="企业名称">{{ form.company.name }}</el-descriptions-item>
            <el-descriptions-item label="统一社会信用代码">{{ form.company.credit }}</el-descriptions-item>
            <el-descriptions-item label="企业类型">{{ form.company.type }}</el-descriptions-item>
            <el-descriptions-item label="企业领域">{{ form.company.industry }}</el-descriptions-item>
            <el-descriptions-item label="企业规模">{{ form.company.scale }}</el-descriptions-item>
            <el-descriptions-item label="登记状态">{{ form.company.state }}</el-descriptions-item>
            <el-descriptions-item label="企查分">{{ form.company.score }}</el-descriptions-item>
          </el-descriptions>
        </el-card>
        <el-card class="card" shadow="hover">
          <div class="section-title">注册信息</div>
          <el-descriptions :column="2" border>
            <el-descriptions-item label="注册资本">{{ form.company.registerCapital }}</el-descriptions-item>
            <el-descriptions-item label="实缴资本">{{ form.company.realCapital }}</el-descriptions-item>
            <el-descriptions-item label="纳税人资质">{{ form.company.qualification }}</el-descriptions-item>
            <el-descriptions-item label="成立日期">{{ form.company.date }}</el-descriptions-item>
            <el-descriptions-item label="营业期限">{{ form.company.ddl }}</el-descriptions-item>
          </el-descriptions>
        </el-card>
        <el-card class="card" shadow="hover">
          <div class="section-title">经营信息</div>
          <el-descriptions :column="2" border>
            <el-descriptions-item label="人员规模">{{ form.company.employeeScale }}</el-descriptions-item>
            <el-descriptions-item label="参保人数">{{ form.company.insuredEmployee }}</el-descriptions-item>
            <el-descriptions-item label="国标行业">{{ form.company.nationalStandard }}</el-descriptions-item>
            <el-descriptions-item label="经营范围">{{ form.company.businessScope }}</el-descriptions-item>
            <el-descriptions-item label="企业标签">
              <el-tag v-for="tag in form.company.tag" :key="tag" type="info" style="margin-right: 5px">{{ tag }}</el-tag>
            </el-descriptions-item>
          </el-descriptions>
        </el-card>
      </div>
      <div class="loading">
        <svg v-if="form.loading" class="loading rect" width="50" height="50" viewbox="0 0 50 50">
          <polygon points="0 0 0 50 50 50 50 0" class="polygon" />
        </svg>
      </div>
    </div>
  </div>

</template>

<script setup>
import {onMounted, reactive} from 'vue'
import {useRoute} from "vue-router";
import logo from '@/assets/logo.png'
import {post} from "@/net/index.js";

const route = useRoute()

const form = reactive({
  company: {
    uid: route.params.uid,
    account: '',
    name: '',
    industry: '',
    scale: '',
    state: '',
    score: '',
    credit: '',
    tag: [],
    date: '',
    ddl: '',
    qualification: '',
    registerCapital: '',
    realCapital: '',
    type: '',
    employeeScale: '',
    insuredEmployee: '',
    nationalStandard: '',
    businessScope: ''
  },
  loading: true
})

function getCompanyDetail() {
  post('/Company/GetCompanyInfo', {
    uid: form.company.uid
  }, (data) => {
    form.company = data
    form.loading = false
  })
}

onMounted(() => {
  getCompanyDetail()
})
</script>

<style scoped>
.company {
  display: flex;
  height: 100vh;
  flex-direction: column;
}

.headLine {
  background-color: #191a1f;
  height: 120px;
  color: white;
  width: 100%;
  margin-bottom: 20px;
  display: flex;
  flex-direction: row;
}

.headImg {
  width: 100px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.headTitle {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
}

.company-detail {
  flex: 1;
  padding: 20px;
  display: flex;
  flex-direction: column;
  gap: 20px;
  align-items: center;
}

.card {
  padding: 10px;
  margin-bottom: 15px;
}

.section-title {
  font-size: 18px;
  font-weight: bold;
  margin-bottom: 10px;
}

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
</style>
