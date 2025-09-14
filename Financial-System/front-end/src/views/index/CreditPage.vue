<script setup>
import { onMounted, reactive, ref, onUnmounted, nextTick } from "vue";
import * as echarts from 'echarts'
import { StarFilled } from "@element-plus/icons-vue";
import { getTokenHeader, post, postMultipart } from "@/net/index.js";
import axios from "axios";
import { ElMessage } from "element-plus";
import router from "@/router/index.js";

const form = reactive({
  showOutput1: false,
  showOutput2: false,
  loading1: false,
  loading2: false,
  input1: {
    debt_rate: '',        // 资产负债率（%）
    interest: '',         // EBITDA利息数（倍）
    inventory_turn: '',   // 存货周转次数（次）
    flow_debt: '',        // 现金流动负债比率或EBITDA（%）
    cost: '',             // 成本费用利润率（%）
    owner_equity: ''      // 所有者权益合计（万元）
  },
  output1: {
    debt_rate: [],
    interest: [],
    inventory_turn: [],
    flow_debt: [],
    cost: [],
    owner_equity: []
  },
  input2: {
    target_file: ''
  },
  output2: {
    star: '',                   // 星级
  },
  template_file: ''
})

const chartContainer1 = ref(null)
const chartContainer2 = ref(null)
const chartContainer3 = ref(null)
const chartContainer4 = ref(null)
const chartContainer5 = ref(null)
const chartContainer6 = ref(null)

onMounted(() => {
  getTemplateFile()
})

// 获取模版文件
function getTemplateFile() {
  const header = getTokenHeader()
  if (!header) {
    // token 无效，不能发送本次请求
    ElMessage.warning('登录状态过期，请重新登录')
    router.push('/')
    return
  }


  axios.get('/Model/GetTemplate', {
    responseType: 'blob',
    headers: header
  }).then(response => {
    form.template_file = new Blob([response.data], { type: 'text/csv' });
  }).catch(error => {
    ElMessage.warning('CSV 模版文件获取失败')
  });
}

// 获取决策树评估结果
function getDecisionTreeRes() {
  form.showOutput1 = true
  form.loading1 = true

  const d = {
    debt_rate: Number(form.input1.debt_rate),
    interest: Number(form.input1.interest),
    inventory_turn: Number(form.input1.inventory_turn),
    flow_debt: Number(form.input1.flow_debt),
    cost: Number(form.input1.cost),
    owner_equity: Number(form.input1.owner_equity)
  }

  post('/Model/GetCredit', d, (data) => {
    form.output1 = data
    form.loading1 = false
    nextTick(() => {
      const chart1 = echarts.init(chartContainer1.value)
      const chart2 = echarts.init(chartContainer2.value)
      const chart3 = echarts.init(chartContainer3.value)
      const chart4 = echarts.init(chartContainer4.value)
      const chart5 = echarts.init(chartContainer5.value)
      const chart6 = echarts.init(chartContainer6.value)
      chart1.setOption(getOption(form.output1.debt_rate, '资产负债率', '%'))
      chart2.setOption(getOption(form.output1.interest, 'EBITDA利息数', '倍'))
      chart3.setOption(getOption(form.output1.inventory_turn, '存货周转次数', '次'))
      chart4.setOption(getOption(form.output1.flow_debt, '现金流动负债比率或EBITDA', '%'))
      chart5.setOption(getOption(form.output1.cost, '成本费用利润率', '%'))
      chart6.setOption(getOption(form.output1.owner_equity, '所有者权益合计', '万元'))
    })
  })
}

// 获取CNN评估结果
function getCnnRes() {
  form.showOutput2 = true
  form.loading2 = true
  const formData = new FormData();
  formData.append('targetFile', form.input2.target_file);


  postMultipart('/Model/GetCnnPredict', formData, (data) => {
    form.output2 = data
    form.loading2 = false
  })
}

function getPositionLabel(data) {
  if (data[5] <= data[0]) return '≤最小值'
  if (data[5] <= data[1]) return '0-25%'
  if (data[5] <= data[2]) return '25-50%'
  if (data[5] <= data[3]) return '50-75%'
  if (data[5] <= data[4]) return '75-100%'
  return '>最大值'
}

function getOption(data, title, metric) {
  let positionLabel = getPositionLabel(data)
  return {
    title: {
      text: title,
      left: 'center'
    },
    tooltip: {
      trigger: 'item',
      formatter: (params) => {
        if (params.seriesName === '样本点') {
          return `数值: ${params.value[1].toFixed(1)}<br/>位置: ${positionLabel}`
        }
        return `
          最小值: ${data[0]}<br/>
          Q1: ${data[1]}<br/>
          中位数: ${data[2]}<br/>
          Q3: ${data[3]}<br/>
          最大值: ${data[4]}
        `
      }
    },
    xAxis: {
      type: 'category',
      data: [''],
      axisLabel: {
        interval: 0
      }
    },
    yAxis: {
      type: 'value',
      name: metric,
      min: data[0], // 留出一些空间
      max: data[4]
    },
    series: [
      {
        name: '箱线图',
        type: 'boxplot',
        data: [data.slice(0, 5)],
        itemStyle: {
          color: '#5470C6',
          borderColor: '#5470C6'
        }
      },
      {
        name: '样本点',
        type: 'scatter',
        data: [[0, data[5]]], // 直接使用已知的sampleValue
        symbolSize: 12,
        itemStyle: {
          color: '#EE6666'
        },
        label: {
          show: true,
          formatter: `${data[5].toFixed(1)}`,
          position: 'top',
          color: '#EE6666'
        }
      }
    ]
  }
}

function downloadFile() {
  const url = URL.createObjectURL(form.template_file)
  const a = document.createElement('a')
  a.href = url
  a.download = 'template.csv'
  document.body.appendChild(a)
  a.click()
  document.body.removeChild(a)
  URL.revokeObjectURL(url)
}

function handleFileUpload(event) {
  const file = event.target.files[0]
  if (file) {
    form.input2.target_file = file
  }
}

</script>

<template>
  <div class="credit-container">
    <!-- 决策树评估部分 -->
    <div class="evaluation-card decision-tree">
      <div class="card-header">
        <h2>基于决策树的企业信用等级评估</h2>
      </div>

      <div class="card-body">
        <div class="input-form">
          <div class="form-row">
            <div class="form-item">
              <label>资产负债率（%）</label>
              <el-input v-model="form.input1.debt_rate" type="number" placeholder="请输入百分比" />
            </div>
            <div class="form-item">
              <label>EBITDA利息数（倍）</label>
              <el-input v-model="form.input1.interest" type="number" placeholder="请输入倍数" />
            </div>
          </div>

          <div class="form-row">
            <div class="form-item">
              <label>存货周转次数（次）</label>
              <el-input v-model="form.input1.inventory_turn" type="number" placeholder="请输入次数" />
            </div>
            <div class="form-item">
              <label>现金流动负债比率或EBITDA（%）</label>
              <el-input v-model="form.input1.flow_debt" type="number" placeholder="请输入百分比" />
            </div>
          </div>

          <div class="form-row">
            <div class="form-item">
              <label>成本费用利润率（%）</label>
              <el-input v-model="form.input1.cost" type="number" placeholder="请输入百分比" />
            </div>
            <div class="form-item">
              <label>所有者权益合计（万元）</label>
              <el-input v-model="form.input1.owner_equity" type="number" placeholder="请输入金额" />
            </div>
          </div>

          <div class="form-actions">
            <el-button type="primary" @click="getDecisionTreeRes" class="submit-btn">
              开始评估
            </el-button>
          </div>
        </div>

        <div v-if="form.showOutput1" class="result-container">
          <div v-if="form.loading1" class="loading-container">
            <div class="loading-spinner"></div>
            <span>评估中，请稍候...</span>
          </div>

          <div v-if="!form.loading1" class="result-content">
            <h3 class="result-title">企业指标行业对比分析结果</h3>
            <div class="charts-grid">
              <div ref="chartContainer1" class="chart-box"></div>
              <div ref="chartContainer2" class="chart-box"></div>
              <div ref="chartContainer3" class="chart-box"></div>
              <div ref="chartContainer4" class="chart-box"></div>
              <div ref="chartContainer5" class="chart-box"></div>
              <div ref="chartContainer6" class="chart-box"></div>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- 神经网络评估部分 -->
    <div class="evaluation-card cnn">
      <div class="card-header">
        <h2>结合卷积神经网络与记忆网络的企业信用等级评估</h2>
      </div>

      <div class="card-body">
        <div class="input-form">
          <div class="file-upload-section">
            <el-button type="primary" @click="downloadFile" class="download-btn">
              <i class="el-icon-download"></i>
              下载CSV模板文件
            </el-button>

            <div class="upload-area">
              <label class="custom-file-upload">
                <input type="file" accept=".csv" @change="handleFileUpload" />
                <span class="upload-text">
                  <i class="el-icon-upload"></i>
                  点击或拖拽文件到此处
                </span>
                <span class="file-name" v-if="form.input2.target_file">
                  {{ form.input2.target_file.name }}
                </span>
              </label>
            </div>

            <el-button type="success" @click="getCnnRes" class="submit-btn" :disabled="!form.input2.target_file">
              <i class="el-icon-upload2"></i>
              上传 && 评级
            </el-button>
          </div>
        </div>

        <div v-if="form.showOutput2" class="result-container">
          <div v-if="form.loading2" class="loading-container">
            <div class="loading-spinner"></div>
            <span>评估中，请稍候...</span>
          </div>

          <div v-if="!form.loading2" class="result-content">
            <h3 class="result-title">评估结果</h3>
            <div class="result-messages">
              <div class="message-item">
                <span class="message-label">信用星级：</span>
                <span class="message-content">
                  <el-icon v-for="n in form.output2.star" :key="n" class="star-icon">
                    <StarFilled />
                  </el-icon>
                </span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
/* 基础样式 */
.credit-container {
  display: flex;
  flex-direction: column;
  gap: 24px;
  padding: 20px;
  max-width: 1200px;
  margin: 0 auto;
  font-family: 'Helvetica Neue', Arial, sans-serif;
}

/* 卡片样式 */
.evaluation-card {
  background: #ffffff;
  border-radius: 8px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
  overflow: hidden;
  transition: all 0.3s ease;
}

.evaluation-card:hover {
  box-shadow: 0 4px 16px 0 rgba(0, 0, 0, 0.15);
}

.card-header {
  padding: 16px 24px;
  background: linear-gradient(135deg, #409EFF, #337ecc);
  color: white;
}

.card-header h2 {
  margin: 0;
  font-size: 18px;
  font-weight: 500;
}

.card-body {
  padding: 24px;
}

/* 表单样式 */
.input-form {
  margin-bottom: 20px;
}

.form-row {
  display: flex;
  gap: 20px;
  margin-bottom: 16px;
}

.form-item {
  flex: 1;
  display: flex;
  flex-direction: column;
}

.form-item label {
  margin-bottom: 8px;
  font-size: 14px;
  color: #606266;
  font-weight: 500;
}

.form-actions {
  display: flex;
  justify-content: flex-end;
  margin-top: 20px;
}

.submit-btn {
  padding: 10px 24px;
}

/* 文件上传区域 */
.file-upload-section {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.upload-area {
  border: 2px dashed #dcdfe6;
  border-radius: 6px;
  padding: 20px;
  text-align: center;
  transition: border-color 0.3s;
}

.upload-area:hover {
  border-color: #409EFF;
}

.custom-file-upload {
  display: flex;
  flex-direction: column;
  align-items: center;
  cursor: pointer;
}

.upload-text {
  display: flex;
  align-items: center;
  gap: 8px;
  color: #606266;
  font-size: 14px;
}

.file-name {
  margin-top: 8px;
  color: #409EFF;
  font-size: 13px;
}

input[type="file"] {
  display: none;
}

.download-btn {
  align-self: flex-start;
}

/* 结果区域 */
.result-container {
  margin-top: 24px;
  border-top: 1px solid #ebeef5;
  padding-top: 24px;
}

.loading-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 40px 0;
}

.loading-spinner {
  width: 40px;
  height: 40px;
  border: 4px solid #f3f3f3;
  border-top: 4px solid #409EFF;
  border-radius: 50%;
  animation: spin 1s linear infinite;
  margin-bottom: 16px;
}

@keyframes spin {
  0% {
    transform: rotate(0deg);
  }

  100% {
    transform: rotate(360deg);
  }
}

.result-title {
  color: #303133;
  font-size: 16px;
  margin-bottom: 16px;
  padding-bottom: 8px;
  border-bottom: 1px solid #ebeef5;
}

.charts-grid {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 20px;
  margin-top: 20px;
}

.chart-box {
  height: 300px;
  background: #f9f9f9;
  border-radius: 4px;
}

.result-messages {
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.message-item {
  display: flex;
  gap: 8px;
}

.message-label {
  font-weight: 500;
  color: #606266;
}

.message-content {
  color: #303133;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .form-row {
    flex-direction: column;
    gap: 16px;
  }

  .charts-grid {
    grid-template-columns: 1fr;
  }
}

.star-icon {
  color: gold;
  font-size: 24px;
  /* 可选，改变大小 */
  margin-right: 3px;
  /* 可选，调整星星间距 */
}
</style>