<script setup>
import {onMounted, reactive} from "vue";
import {ArrowDownBold, ArrowUpBold, Clock} from "@element-plus/icons-vue";
import { use } from 'echarts/core'
import { CanvasRenderer } from 'echarts/renderers'
import { BarChart, LineChart } from 'echarts/charts'
import {
  TitleComponent,
  TooltipComponent,
  GridComponent,
  LegendComponent
} from 'echarts/components'
import VChart from 'vue-echarts'
import router from "@/router/index.js";
import {get, post} from "@/net/index.js";
use([
  CanvasRenderer,
  BarChart,
  LineChart,
  TitleComponent,
  TooltipComponent,
  GridComponent,
  LegendComponent
])

function checkNewsDetail(id) {
  router.push({
    name: 'news-content',
    params: {
      id: id
    }
  })
}

const form = reactive({
  industries: [
      '商业', '高压临电', '房地产经营开发',
      '其他服务', '食品加工', '仓储物流',
      '集成电路', '设备制造', '材料加工',
      '办公', '城市基础设施', '汽车',
      '数据服务', '生物医药', '教育科研',
      '充换电服务业'
  ],
  companies: [],
  companyInfo: {
    yearList: [],    // 年份顺序
    profits: [],     // 归母净利润
    rate: [],        // 同比
    money: [],       // 总资产
    debt: [],        // 总负债
    debt_rate: [],   // 负债率
    relatedNews: [],        // 与该公司相关的新闻
    stackInfo: {
      dateList: [],     // 日期列表
      value: []     // 市值
    }                      // 公司股票信息
  },
  openedIndustry: [ 0 ],
  openedCompany: [ ],
  industryIndex: 0,
  companyIndex: 0,
  loading:true,
  navLoading: true,
  showIndustry: true,
  industryInfo:{
    chart: {
      date: [],
      volume: []
    },
    table:[]
  },
  option: {},
  option1: {},
  option2: {},
  option3: {}
})

// 查看行业图表
function checkIndustryChart(index) {
  form.industryIndex = index
  form.showIndustry = true
  form.loading = true
  getIndustryInfo(form.industries[index])
}

// 获取绘制行业图表的信息
function getIndustryInfo(industry_name) {
  post('/Graph/GetIndustryGraphData', {
    industry: industry_name
  }, (data) => {
    form.industryInfo = data
    form.option = {
      title: {
        text: '行业市值',
        left: 'center'
      },
      tooltip: {
        trigger: 'axis',
        axisPointer: {
          type: 'shadow'
        }
      },
      grid: {
        left: '3%',
        right: '4%',
        bottom: '3%',
        containLabel: true
      },
      xAxis: {
        type: 'category',
        data: form.industryInfo.chart.date,
        axisLabel: {
          rotate: 30,
        }
      },
      yAxis: {
        type: 'value'
      },
      series: [
        {
          name: '市值',
          type: 'bar',
          data: form.industryInfo.chart.volume,
          itemStyle: {
            color: '#5470C6'
          },
          label: {
            show: true,
            position: 'top'
          }
        }
      ]
    }
    form.loading = false
  })
}

function clickIndustry(index){
  const i = form.openedIndustry.indexOf(index)
  if(i !== -1) {
    form.openedIndustry.splice(i, 1)
  }
  else {
    form.openedIndustry.push(index)
  }
}

function openCompany(index) {
  const i = form.openedCompany.indexOf(index)
  if(i !== -1) {
    form.openedCompany.splice(i, 1)
  }
  else {
    form.openedCompany.push(index)
  }
}

// 点击查看公司图表信息
function checkCompanyInfo(c_index){
  form.showIndustry = false
  form.companyIndex = c_index
  form.loading = true
  getCompanyInfo(form.companies[c_index].uid)
}

// 获取公司绘图信息
function getCompanyInfo(uid) {
  post('/Graph/GetListedCompanyGraphData', { uid: uid }, (data) => {
    form.companyInfo.yearList = data.yearList
    form.companyInfo.profits = data.profits
    form.companyInfo.rate = data.rate
    form.companyInfo.money = data.money
    form.companyInfo.debt = data.debt
    form.companyInfo.debt_rate = data.debt_rate

    post('/Company/GetCompanyNews', { uid: uid }, (data) => {
      form.companyInfo.relatedNews = data.news

      post('/Company/GetStockChange', { uid: uid }, (data) => {
        form.companyInfo.stackInfo = data
        form.option1 = {
          title: {
            text: '利润图',
            left: 'center'
          },
          tooltip: {
            trigger: 'axis',
            formatter: (params) => {
              let result = `${params[0].axisValue}<br/>`
              params.forEach(item => {
                // 使用对应系列的颜色和形状
                const marker = `
        <span style="
          display:inline-block;
          margin-right:5px;
          border-radius:${item.seriesType === 'bar' ? '2px' : '50%'};
          width:10px;
          height:10px;
          background-color:${item.color};
        "></span>
      `
                const unit = item.seriesName === '归母净利润' ? '亿' : '%'
                result += `${marker} ${item.seriesName}: ${item.data}${unit}<br/>`
              })
              return result
            }
          },
          legend: {
            data: ['归母净利润', '同比'],
            top: 30
          },
          grid: {
            left: '10%',
            right: '10%',
            bottom: '10%',
            containLabel: true
          },
          xAxis: {
            type: 'category',
            data: form.companyInfo.yearList,
            axisTick: {
              alignWithLabel: true
            }
          },
          yAxis: [
            {
              type: 'value',
              name: '归母净利润(亿)',
              min: 0,
              axisLine: {
                show: true
              },
              axisLabel: {
                formatter: '{value}'
              },
              axisPointer: {
                show: false  // 隐藏y轴值指示线
              }
            },
            {
              type: 'value',
              name: '同比(%)',
              min: 0,
              axisLine: {
                show: true
              },
              axisLabel: {
                formatter: '{value}%'
              },
              axisPointer: {
                show: false  // 隐藏y轴值指示线
              }
            }
          ],
          series: [
            {
              name: '归母净利润',
              type: 'bar',
              data: form.companyInfo.profits,
              itemStyle: {
                color: '#5470C6'
              },
              label: {
                show: true,
                position: 'top',
                formatter: '{c}亿'
              }
            },
            {
              name: '同比',
              type: 'line',
              yAxisIndex: 1,
              data: form.companyInfo.rate,
              symbol: 'circle',
              symbolSize: 8,
              itemStyle: {
                color: '#edbd0c'
              },
              label: {
                show: true,
                formatter: '{c}%'
              },
              lineStyle: {
                width: 3
              }
            }
          ]
        }
        form.option2 = {
          title: {
            text: '资产负债图',
            left: 'center'
          },
          tooltip: {
            trigger: 'axis',
            formatter: (params) => {
              let result = `${params[0].axisValue}<br/>`
              params.forEach(item => {
                // 使用对应系列的颜色设置marker样式
                const marker = `
        <span style="
          display:inline-block;
          margin-right:5px;
          border-radius:${item.seriesType === 'bar' ? '2px' : '50%'};
          width:10px;
          height:10px;
          background-color:${item.color};
        "></span>
      `
                const unit = item.seriesName === '负债率' ? '%' : '亿'
                result += `${marker} ${item.seriesName}: ${item.data}${unit}<br/>`
              })
              return result
            }
          },
          legend: {
            data: ['总资产', '总负债', '负债率'],
            top: 30
          },
          grid: {
            left: '10%',
            right: '10%',
            bottom: '10%',
            containLabel: true
          },
          xAxis: {
            type: 'category',
            data: form.companyInfo.yearList,
            axisTick: {
              alignWithLabel: true
            }
          },
          yAxis: [
            {
              type: 'value',
              name: '金额(亿)',
              min: 0,
              axisLine: {
                show: true
              },
              axisLabel: {
                formatter: '{value}'
              },
              axisPointer: {
                show: false
              }
            },
            {
              type: 'value',
              name: '负债率(%)',
              min: 0,
              max: 100,
              axisLine: {
                show: true
              },
              axisLabel: {
                formatter: '{value}%'
              },
              axisPointer: {
                show: false
              }
            }
          ],
          series: [
            {
              name: '总资产',
              type: 'bar',
              barGap: 0,
              barWidth: '30%',
              data: form.companyInfo.money,
              itemStyle: {
                color: '#5470C6'
              },
              label: {
                show: true,
                position: 'top',
                formatter: '{c}亿'
              }
            },
            {
              name: '总负债',
              type: 'bar',
              barWidth: '30%',
              data: form.companyInfo.debt,
              itemStyle: {
                color: '#EE6666'
              },
              label: {
                show: true,
                position: 'top',
                formatter: '{c}亿'
              }
            },
            {
              name: '负债率',
              type: 'line',
              yAxisIndex: 1,
              data: form.companyInfo.debt_rate,
              symbol: 'circle',
              symbolSize: 8,
              itemStyle: {
                color: '#edbd0c'
              },
              label: {
                show: true,
                formatter: '{c}%'
              },
              lineStyle: {
                width: 3
              }
            }
          ]
        }
        form.option3 = {
          title: {
            text: '股票市值',
            left: 'center'
          },
          tooltip: {
            trigger: 'axis',
            formatter: (params) => {
              let result = `${params[0].axisValue}<br/>`
              params.forEach(item => {
                const marker = `
          <span style="
            display:inline-block;
            margin-right:5px;
            border-radius:50%;
            width:10px;
            height:10px;
            background-color:${item.color};
          "></span>
        `
                result += `${marker} ${item.seriesName}: ${item.data}亿<br/>`
              })
              return result
            }
          },
          grid: {
            left: '10%',
            right: '10%',
            bottom: '10%',
            containLabel: true
          },
          xAxis: {
            type: 'category',
            name: '日期',
            data: form.companyInfo.stackInfo.dateList,
            axisTick: {
              alignWithLabel: true
            }
          },
          yAxis: {
            type: 'value',
            name: '市值(亿)',
            axisLine: {
              show: true
            },
            axisLabel: {
              formatter: '{value}'
            },
            axisPointer: {
              show: false
            }
          },
          series: [
            {
              name: '市值',
              type: 'line',
              data: form.companyInfo.stackInfo.value,
              symbol: 'circle',
              symbolSize: 8,
              itemStyle: {
                color: '#5470C6'
              },
              label: {
                show: true,
                formatter: '{c}亿'
              },
              lineStyle: {
                width: 3
              }
            }
          ]
        }
        form.loading = false
      })
    })
  })
}

// 获取导航区的对应行业的上市公司
function getNavInfo(){
  form.navLoading = true
  get('/Company/GetListedCompanies', (data) => {
    form.companies = data.companies
    form.navLoading=false
    getIndustryInfo(form.industries[0])
  })
}

// 初始化
onMounted(() => {
  getNavInfo()
})

</script>


<template>
  <div class="chart">
    <div class="nav">
      <div v-if="!form.navLoading" style="margin-top: 40px;margin-left: 40px;margin-bottom: 60px">
        <div style="margin-bottom: 20px" v-for="(industry, index) in form.industries">
          <div @click="clickIndustry(index)" class="industry" :class="{selected: form.openedIndustry.indexOf(index) !== -1}">
            <el-icon v-if="form.openedIndustry.indexOf(index) === -1" style="font-size: 20px"><ArrowDownBold /></el-icon>
            <el-icon v-if="form.openedIndustry.indexOf(index) !== -1" style="font-size: 20px;"><ArrowUpBold /></el-icon>
            <span style="font-size: 20px;margin-left: 5px">{{industry}}</span>
          </div>


          <div style="margin-left: 35px;margin-top: 5px;margin-bottom: 15px" v-if="form.openedIndustry.indexOf(index) !== -1">

            <div @click="checkIndustryChart(index)" class="chart-industry" :class="{selected: form.showIndustry && form.industryIndex === index}">
              <span>行业分析图表</span>
            </div>


            <div @click="openCompany(index)" class="company" :class="{selected: form.openedCompany.indexOf(index) !== -1}" style="margin-top: 5px">
              <el-icon v-if="form.openedCompany.indexOf(index) === -1" style="font-size: 16px"><ArrowDownBold /></el-icon>
              <el-icon v-if="form.openedCompany.indexOf(index) !== -1" style="font-size: 16px;"><ArrowUpBold /></el-icon>
              <span style="margin-left: 5px">公司股票分析</span>
            </div>

            <div class="allTheCompanies" v-if="form.openedCompany.indexOf(index) !== -1">
              <div v-for="(company,c_index) in form.companies" style="margin-left: 35px;margin-top: 5px">
                <div v-if="company.industry === industry">
                  <span @click="checkCompanyInfo(c_index)" class="companyName" :class="{selected:!form.showIndustry&&form.companyIndex===c_index}">{{company.name}}</span>
                </div>
              </div>
            </div>
          </div>

          <hr style="margin-top: 20px"/>
        </div>
      </div>

      <div style="margin-top: 40vh;margin-left: 100px" class="loading">
        <svg v-if="form.navLoading" class="loading rect" width="50" height="50" viewbox="0 0 50 50">
          <polygon points="0 0 0 50 50 50 50 0" class="polygon" />
        </svg>
      </div>
    </div>


    <div class="ctt">

      <div v-if="form.showIndustry" class="industryChart">
        <div v-if="!form.loading" style="padding: 10px 10px 10px 10px" class="industry-chart">
          <v-chart class="chart" :option="form.option" style="height: 100%;width: 100%"/>
        </div>
        <hr v-if="!form.loading" style="width: 100%;margin-bottom: 30px"/>
        <div v-if="!form.loading" class="industry-table">
          <el-table v-if="!form.loading" :data="form.industryInfo.table" style="width: 90%; flex: 1;margin-bottom: 10px">
            <el-table-column prop="date" label="日期"></el-table-column>
            <el-table-column prop="code" label="行业代码"></el-table-column>
            <el-table-column prop="open" label="开盘价"></el-table-column>
            <el-table-column prop="close" label="收盘价"></el-table-column>
            <el-table-column prop="turnOver" label="成交量"></el-table-column>
            <el-table-column prop="totalShare" label="总股价"></el-table-column>
          </el-table>
        </div>

        <div class="loading">
          <svg v-if="form.loading" class="loading rect" width="50" height="50" viewbox="0 0 50 50">
            <polygon points="0 0 0 50 50 50 50 0" class="polygon" />
          </svg>
        </div>
      </div>

      <div v-if="!form.showIndustry" class="companyChart">
        <div v-if="!form.loading" class="top">
          <div class="related-news">
            <div v-for="article in form.companyInfo.relatedNews">
              <div @click="checkNewsDetail(article.id)" class="article">
                <img v-if="article.photo_url !== ''" class="article-img" :src="article.photo_url" alt="photo"/>
                <div style="display: flex;flex-direction: column">
                  <h3>{{article.title}}</h3>
                  <div style="display: flex;flex-direction: row;color: grey;align-items: center">
                    <el-icon><Clock /></el-icon>
                    <span style="margin-left: 5px">{{article.time}}</span>
                  </div>
                </div>
              </div>
            </div>
          </div>
          <div class="stack-chart">
            <v-chart class="chart" :option="form.option3" style="height: 100%;width: 100%" />
          </div>
        </div>
        <hr v-if="!form.loading" style="width: 100%;margin-bottom: 30px"/>
        <div v-if="!form.loading" class="bottom">
          <div style="padding: 10px 10px 10px 10px" class="company-profit">
            <v-chart class="chart" :option="form.option1" style="height: 100%;width: 100%" />
          </div>

          <div class="company-debt">
            <v-chart class="chart" :option="form.option2" style="height: 100%;width: 100%" />
          </div>
        </div>
        <div class="loading">
          <svg v-if="form.loading" class="loading rect" width="50" height="50" viewbox="0 0 50 50">
            <polygon points="0 0 0 50 50 50 50 0" class="polygon" />
          </svg>
        </div>
      </div>
    </div>
  </div>
</template>


<style scoped>
.industry,
.companyName,
.chart-industry,
.company {
  transition: all 0.3s ease;
  color: grey;
}

.company.selected{
  color: #1976d2;
}

.companyName.selected{
  color: #1976d2;
  text-decoration: underline;
  text-decoration-color: #1976d2;
}

.industryChart,
.companyChart {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 100%;
  width: 100%;
}

.industry-chart,
.industry-table{
  flex: 1;
}

.industry-table{
  overflow-y: auto;
  max-height: 100%;
  width: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
}

.company-debt,
.company-profit{
  flex: 1;
}


.stack-chart{
  flex: 1;
  height: 100%;
  width: 100%;
}

.related-news{
  overflow-y: auto;
  max-height: 400px;
  width: 400px;
}


.industry-chart{
  height: 100%;
  width: 100%;
}

.companyName:hover,
.industry:hover,
.chart-industry:hover,
.company:hover {
  color: black;
}

.industry.selected{
  color: #1976d2;
}

.top,
.bottom{
  flex: 1;
  height: 100%;
  width: 100%;
  display: flex;
  flex-direction: row;
}



.chart-industry.selected{
  color: #1976d2;
  text-decoration: underline;
  text-decoration-color: #1976d2;
}

.chart{
  height: 92vh;
  width: 100%;
  display: flex;
  flex-direction: row;
}

.nav{
  width: 300px;
  overflow-y: auto;
  max-height: 100%;
  margin-right: 50px;
  cursor: pointer;
}

.ctt{
  flex: 1;
  margin-top: 30px;
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

.article {
  display: flex;
  flex-direction: row;
  align-items: center;
  margin-bottom: 30px;
  transition: all 0.3s ease-in;
}

.article-img {
  width: 150px;
  height: 110px;
  margin-right: 20px;
  margin-left: 20px;
}

.article:hover{
  border-radius: 20px;
  background: #f5f5f5;
  box-shadow: inset 20px 20px 60px #d0d0d0,
  inset -20px -20px 60px #ffffff;
}

.article:hover h3{
  color: blue;
}

h3{
  transition: all 0.3s ease-in;
}
</style>