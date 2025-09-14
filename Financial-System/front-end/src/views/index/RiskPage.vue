<script setup>
import { reactive } from "vue";
import { use } from 'echarts/core'
import { CanvasRenderer } from 'echarts/renderers'
import { RadarChart } from 'echarts/charts'
import {
  TitleComponent,
  TooltipComponent,
  RadarComponent
} from 'echarts/components'
import VChart from 'vue-echarts'
import { post } from "@/net/index.js";
use([
  CanvasRenderer,
  RadarChart,
  TitleComponent,
  TooltipComponent,
  RadarComponent
])

const form = reactive({
  showOutput1: false,
  showOutput2: false,
  loading1: false,
  loading2: false,
  input1: {
    i1: '',    // 流动资产周转率
    i2: '',    // 应收账款周转率
    i3: '',    // 纳税信用等级
    i4: '',    // 1/财务费用率
    i5: '',    // 1/产权比率
    i6: '',    // 流动比率
    i7: '',    // 资产负债率
    i8: '',    // 利息保障倍数
    i9: '',    // 现金流量利息保障倍数
    i10: '',   // 营业收入增长率
    i11: '',   // 总资产增长率
  },
  output1: {
    o1: 0.0122,   // 流动性
    o2: 0.9321,   // 筹资
    o3: 0.674,   // 清偿
    o4: 0.5165,   // 成长力
    o5: 0.5521,   // 财务风险综合指标
  },
  input2: {
    i1: '',    // 销售净利率
    i2: '',    // 总资产报酬率
    i3: '',    // 净资产收益率
    i4: '',    // 存货周转率
    i5: '',    // 总资产周转率
    i6: '',    // 成本费用利润率
    i7: '',    // 诉讼数量(个)
    i8: '',    // 经营异常次数
    i9: '',    // 行政处罚次数
    i10: '',    // 动产质押次数
    i11: '',    // 自身风险
    i12: '',    // 周边风险
    i13: '',    // 股东数量（注册）
    i14: '',    // 第一大股东持股比例（注册）
  },
  output2: {
    o1: 0.6570,   // 经营力
    o2: 0.9221,   // 司法
    o3: 0.0923,   // 组织
    o4: 0.3528,   // 经营风险综合指标
  },
  option1: {},
  option2: {},
  indicator1: [
    { name: '流动性', max: 1 },
    { name: '筹资', max: 1 },
    { name: '清偿', max: 1 },
    { name: '成长力', max: 1 },
  ],
  indicator2: [
    { name: '经营力', max: 1 },
    { name: '司法', max: 1 },
    { name: '组织', max: 1 },
  ]
})

// 获取财务风险评估结果
function getFinanceRisk() {
  form.showOutput1 = true
  form.loading1 = true

  const d = [Number(form.input1.i1),
  Number(form.input1.i2),
  Number(form.input1.i3),
  Number(form.input1.i4),
  Number(form.input1.i5),
  Number(form.input1.i6),
  Number(form.input1.i7),
  Number(form.input1.i8),
  Number(form.input1.i9),
  Number(form.input1.i10),
  Number(form.input1.i11)]

  post('/Model/GetFinancialRisk', { input: d }, (data) => {
    form.output1 = data
    form.option1 = getOption(form.indicator1, [form.output1.o1, form.output1.o2, form.output1.o3, form.output1.o4], '财务风险评估结果', '财务风险评估指标')
    form.loading1 = false
  })
}

// 获取经营风险评估结果
function getManagementRisk() {
  form.showOutput2 = true
  form.loading2 = true

  const d = [Number(form.input2.i1),
  Number(form.input2.i2),
  Number(form.input2.i3),
  Number(form.input2.i4),
  Number(form.input2.i5),
  Number(form.input2.i6),
  Number(form.input2.i7),
  Number(form.input2.i8),
  Number(form.input2.i9),
  Number(form.input2.i10),
  Number(form.input2.i11),
  Number(form.input2.i12),
  Number(form.input2.i13),
  Number(form.input2.i14)]

  post('/Model/GetBusinessRisk', { input: d }, (data) => {
    form.output2 = data
    form.option2 = getOption(form.indicator2, [form.output2.o1, form.output2.o2, form.output2.o3], '经营风险评估结果', '经营风险评估指标')
    form.loading2 = false
  })
}

function getOption(indicator, data, title, data_name) {
  return {
    title: {
      text: title,
      left: 'center',
      textStyle: {
        color: '#333',
        fontSize: 16
      }
    },
    tooltip: {
      trigger: 'item'
    },
    radar: {
      indicator: indicator,
      radius: '70%',
      axisName: {
        color: '#666',
        fontSize: 12
      },
      splitArea: {
        areaStyle: {
          color: ['rgba(200, 200, 200, 0.1)', 'rgba(200, 200, 200, 0.2)'], // 淡化的背景色
          shadowColor: 'rgba(0, 0, 0, 0.05)',
          shadowBlur: 5
        }
      },
      axisLine: {
        lineStyle: {
          color: 'rgba(150, 150, 150, 0.3)' // 淡化轴线
        }
      },
      splitLine: {
        lineStyle: {
          color: 'rgba(150, 150, 150, 0.3)' // 淡化分割线
        }
      }
    },
    series: [
      {
        type: 'radar',
        data: [
          {
            value: data,
            name: data_name,
            areaStyle: {
              color: 'rgba(255, 234, 0, 0.4)' // 半透明填充色
            },
            lineStyle: {
              width: 2,
              color: '#5470C6'
            },
            symbolSize: 6,
            label: {
              show: true,
              color: '#5470C6',
              formatter: '{c}'
            }
          }
        ]
      }
    ]
  }
}
</script>

<template>
  <div class="risk-assessment-container">
    <!-- 财务风险评估部分 -->
    <div class="assessment-card finance-risk">
      <div class="card-header">
        <h2>财务风险评估</h2>
      </div>

      <div class="card-body">
        <div class="input-form">
          <div class="form-grid">
            <div class="form-item">
              <label>流动资产周转率</label>
              <el-input v-model="form.input1.i1" type="number" placeholder="请输入周转率" />
            </div>
            <div class="form-item">
              <label>应收账款周转率</label>
              <el-input v-model="form.input1.i2" type="number" placeholder="请输入周转率" />
            </div>
            <div class="form-item">
              <label>纳税信用等级</label>
              <el-input v-model="form.input1.i3" type="number" placeholder="请输入等级" />
            </div>
            <div class="form-item">
              <label>1 / 财务费用率</label>
              <el-input v-model="form.input1.i4" type="number" placeholder="请输入数值" />
            </div>
            <div class="form-item">
              <label>1 / 产权比率</label>
              <el-input v-model="form.input1.i5" type="number" placeholder="请输入数值" />
            </div>
            <div class="form-item">
              <label>流动比率</label>
              <el-input v-model="form.input1.i6" type="number" placeholder="请输入比率" />
            </div>
            <div class="form-item">
              <label>资产负债率</label>
              <el-input v-model="form.input1.i7" type="number" placeholder="请输入百分比" />
            </div>
            <div class="form-item">
              <label>利息保障倍数</label>
              <el-input v-model="form.input1.i8" type="number" placeholder="请输入倍数" />
            </div>
            <div class="form-item">
              <label>现金流量利息保障倍数</label>
              <el-input v-model="form.input1.i9" type="number" placeholder="请输入倍数" />
            </div>
            <div class="form-item">
              <label>营业收入增长率</label>
              <el-input v-model="form.input1.i10" type="number" placeholder="请输入百分比" />
            </div>
            <div class="form-item">
              <label>总资产增长率</label>
              <el-input v-model="form.input1.i11" type="number" placeholder="请输入百分比" />
            </div>
          </div>

          <div class="form-actions">
            <el-button type="primary" @click="getFinanceRisk" class="submit-btn">
              <i class="el-icon-search"></i>
              开始评估
            </el-button>
          </div>
        </div>

        <div v-if="form.showOutput1" class="result-container">
          <div v-if="form.loading1" class="loading-container">
            <div class="loading-spinner"></div>
            <span>财务风险评估中，请稍候...</span>
          </div>

          <div v-if="!form.loading1" class="result-content">
            <v-chart class="result-chart" :option="form.option1" />
            <div class="result-summary">
              <h3>财务风险综合指标：<span class="risk-value">{{ form.output1.o5 }}</span></h3>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- 经营风险评估部分 -->
    <div class="assessment-card management-risk">
      <div class="card-header">
        <h2>经营风险评估</h2>
      </div>

      <div class="card-body">
        <div class="input-form">
          <div class="form-grid">
            <div class="form-item">
              <label>销售净利率</label>
              <el-input v-model="form.input2.i1" type="number" placeholder="请输入百分比" />
            </div>
            <div class="form-item">
              <label>总资产报酬率</label>
              <el-input v-model="form.input2.i2" type="number" placeholder="请输入百分比" />
            </div>
            <div class="form-item">
              <label>净资产收益率</label>
              <el-input v-model="form.input2.i3" type="number" placeholder="请输入百分比" />
            </div>
            <div class="form-item">
              <label>存货周转率</label>
              <el-input v-model="form.input2.i4" type="number" placeholder="请输入周转率" />
            </div>
            <div class="form-item">
              <label>总资产周转率</label>
              <el-input v-model="form.input2.i5" type="number" placeholder="请输入周转率" />
            </div>
            <div class="form-item">
              <label>成本费用利润率</label>
              <el-input v-model="form.input2.i6" type="number" placeholder="请输入百分比" />
            </div>
            <div class="form-item">
              <label>诉讼数量(个)</label>
              <el-input v-model="form.input2.i7" type="number" placeholder="请输入数量" />
            </div>
            <div class="form-item">
              <label>经营异常次数</label>
              <el-input v-model="form.input2.i8" type="number" placeholder="请输入次数" />
            </div>
            <div class="form-item">
              <label>行政处罚次数</label>
              <el-input v-model="form.input2.i9" type="number" placeholder="请输入次数" />
            </div>
            <div class="form-item">
              <label>动产质押次数</label>
              <el-input v-model="form.input2.i10" type="number" placeholder="请输入次数" />
            </div>
            <div class="form-item">
              <label>自身风险</label>
              <el-input v-model="form.input2.i11" type="number" placeholder="请输入风险值" />
            </div>
            <div class="form-item">
              <label>周边风险</label>
              <el-input v-model="form.input2.i12" type="number" placeholder="请输入风险值" />
            </div>
            <div class="form-item">
              <label>股东数量（注册）</label>
              <el-input v-model="form.input2.i13" type="number" placeholder="请输入数量" />
            </div>
            <div class="form-item">
              <label>第一大股东持股比例（注册）</label>
              <el-input v-model="form.input2.i14" type="number" placeholder="请输入百分比" />
            </div>
          </div>

          <div class="form-actions">
            <el-button type="primary" @click="getManagementRisk" class="submit-btn">
              <i class="el-icon-search"></i>
              开始评估
            </el-button>
          </div>
        </div>

        <div v-if="form.showOutput2" class="result-container">
          <div v-if="form.loading2" class="loading-container">
            <div class="loading-spinner"></div>
            <span>经营风险评估中，请稍候...</span>
          </div>

          <div v-if="!form.loading2" class="result-content">
            <v-chart class="result-chart" :option="form.option2" />
            <div class="result-summary">
              <h3>经营风险综合指标：<span class="risk-value">{{ form.output2.o4 }}</span></h3>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
/* 基础样式 */
.risk-assessment-container {
  display: flex;
  flex-direction: column;
  gap: 24px;
  padding: 20px;
  max-width: 1400px;
  margin: 0 auto;
  font-family: 'Helvetica Neue', Arial, sans-serif;
}

/* 卡片样式 */
.assessment-card {
  background: #ffffff;
  border-radius: 8px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
  overflow: hidden;
  transition: all 0.3s ease;
}

.assessment-card:hover {
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

.form-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
  gap: 16px;
  margin-bottom: 16px;
}

.form-item {
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
  justify-content: center;
  margin-top: 20px;
}

.submit-btn {
  padding: 10px 32px;
  font-size: 15px;
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

.result-content {
  display: flex;
  flex-direction: column;
  gap: 20px;
}

.result-chart {
  height: 400px;
  width: 100%;
  background: #f9f9f9;
  border-radius: 4px;
  padding: 10px;
}

.result-summary {
  text-align: center;
  padding: 16px;
  background: #f5f7fa;
  border-radius: 4px;
}

.result-summary h3 {
  margin: 0;
  color: #303133;
  font-size: 16px;
}

.risk-value {
  color: #e6a23c;
  font-weight: bold;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .form-grid {
    grid-template-columns: 1fr;
  }

  .card-body {
    padding: 16px;
  }
}
</style>