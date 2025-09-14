<script setup>

import { computed, onMounted, reactive } from "vue";
import router from "@/router/index.js";
import { get } from "@/net/index.js";

const form = reactive({
  industryLabel: [
    '全部',
    '教育科研', '生物医药', '数据服务',
    '建筑业', '设备制造', '材料加工',
    '包装印刷', '房地产经营开发', '医疗',
    '交通运输', '集成电路', '城市基础设施',
    '服饰纺织', '汽车', '其他服务',
    '食品加工', '金融服务', '农业',
    '能源', '商业', '高压临电',
    '充换电服务业', '仓储物流', '办公'
  ],
  selectedLabel: '全部',
  industryInfo: [],
  loading: true
})

// 计算属性来过滤符合当前选定标签的企业
const filteredIndustryInfo = computed(() => {
  if (form.selectedLabel === '全部') {
    return form.industryInfo
  } else {
    return form.industryInfo.filter(item => item.industry === form.selectedLabel)
  }
})

function selectLabel(label) {
  form.selectedLabel = label
}

function getIndustryInfo() {
  get('/Company/GetAllCompaniesInfo', (data) => {
    form.industryInfo = data.companies
    form.loading = false
  })
}

onMounted(() => {
  getIndustryInfo()
})

function checkDetail(company) {
  router.push({
    name: 'company',
    params: {
      uid: company.uid
    }
  })
}
</script>

<template>
  <div class="industry">

    <el-divider>
      <span style="font-size: 15px;color: grey">企业领域</span>
    </el-divider>

    <div class="industry-label">
      <el-tag v-for="label in form.industryLabel" :key="label" @click="selectLabel(label)" class="custom-tag"
        :closable="false" :class="{ selected: form.selectedLabel === label }">
        {{ label }}
      </el-tag>
    </div>

    <el-divider>
      <span style="font-size: 15px;color: grey">企业信息</span>
    </el-divider>

    <div class="industry-list">
      <el-table class="transparent-table" v-if="!form.loading" @row-click="checkDetail" :data="filteredIndustryInfo"
        style="width: 90%;flex: 1;background-color: transparent">
        <el-table-column prop="uid" label="UID"></el-table-column>
        <el-table-column prop="account" label="户号"></el-table-column>
        <el-table-column prop="name" label="企业名称"></el-table-column>
        <el-table-column prop="industry" label="企业领域"></el-table-column>
        <el-table-column prop="scale" label="企业规模"></el-table-column>
        <el-table-column prop="state" label="登记状态"></el-table-column>
        <el-table-column prop="score" label="企查分"></el-table-column>
      </el-table>

      <div class="loading">
        <svg v-if="form.loading" class="loading rect" width="50" height="50" viewbox="0 0 50 50">
          <polygon points="0 0 0 50 50 50 50 0" class="polygon" />
        </svg>
      </div>
    </div>
  </div>
</template>

<style scoped>
.industry {
  display: flex;
  flex-direction: column;
  height: 100%;
}

.industry-label {
  margin-bottom: 15px;
}

.industry-list {
  display: flex;
  flex-direction: column;
  align-items: center;
  flex: 1;
  overflow-y: auto;
  padding: 10px;
  max-height: 450px;
}

::v-deep(.custom-tag:hover) {
  border-radius: 50px;
  background: #893445;
  box-shadow: inset 20px -20px 2px #742c3b,
    inset -20px 20px 2px #9e3c4f;
  cursor: pointer;
}

::v-deep(.custom-tag) {
  height: 50px;
  width: 150px;
  font-size: 16px;
  margin: 10px 10px 10px 10px;
  border-radius: 50px;
  background: #3d3e4d;
  box-shadow: inset 20px -20px 2px #343541,
    inset -20px 20px 2px #464759;
  color: white;
}

::v-deep(.custom-tag.selected) {
  border-radius: 50px;
  background: #893445;
  box-shadow: inset 20px -20px 2px #742c3b,
    inset -20px 20px 2px #9e3c4f;
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