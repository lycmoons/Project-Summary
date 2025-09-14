<template>
    <div class="newsLifeCycle-box">

        <!-- 搜索框 -->
        <el-input v-model="searchKeyword" placeholder="请输入新闻类别" clearable @clear="handleClear"
            @keyup.enter.native="handleSearch" style="width: 300px; margin-bottom: 15px;">
            <template #append>
                <el-button icon="el-icon-search" @click="handleSearch"></el-button>
            </template>
        </el-input>

        <!-- 粒度选择 -->
        <el-select v-model="timeGranularity" placeholder="选择粒度" style="width: 160px; margin-left: 20px;"
            @change="updateChart">

            <el-option label="30分钟" :value="30"></el-option>
            <el-option label="1小时" :value="60"></el-option>
            <el-option label="3小时" :value="180"></el-option>
            <el-option label="6小时" :value="360"></el-option>
            <el-option label="12小时" :value="720"></el-option>
            <el-option label="一天" :value="1440"></el-option>
        </el-select>

        <el-tabs v-model="activeTab" type="border-card">
            <el-tab-pane label="指定类别的新闻生命周期" name="lifeCycle">
                <div class="chart-wrapper">
                    <div id="lifeCycleChart" style="width: 100%; height: 400px;"></div>
                </div>
            </el-tab-pane>
        </el-tabs>

    </div>
</template>

<script>
module.exports = {
    data() {
        return {
            activeTab: 'lifeCycle',
            chartInstance: null,
            searchKeyword: '',
            timeGranularity: 60, // 粒度(分钟)
            rawLifeCycleData: [] // 原始后端返回的时间列表
        }
    },
    mounted() {
        this.initChart()
    },
    methods: {
        initChart() {   //初始化折线图
            this.chartInstance = echarts.init(document.getElementById('lifeCycleChart'))
            const option = {
                title: {
                    text: '新闻类别生命周期曲线',
                    left: 'center',
                    top: 10
                },
                tooltip: {
                    trigger: 'axis'
                },
                grid: {
                    top: 100,
                    left: '10%',
                    right: '10%',
                    bottom: '10%'
                },
                xAxis: {
                    name: '时间',
                    type: 'category',
                    data: []
                },
                yAxis: {
                    name: '个数',
                    type: 'value',

                },
                series: [
                    {
                        name: '个数',
                        type: 'line',
                        data: []
                    }
                ]
            }
            this.chartInstance.setOption(option)
        },

        handleSearch() {
            console.log('搜索关键词:', this.searchKeyword)
            if (!this.searchKeyword) {
                this.$message.warning('请输入新闻类别')
                return
            }
            this.getLifeCycleData()
        },

        handleClear() {
            console.log('搜索框清空了')
            this.rawLifeCycleData = []
            this.updateChart()
        },

        async getLifeCycleData() {

            const category = this.searchKeyword
            try {
                const response = await newsApi.getCategoryLifeCycleData(category);

                //const result = await response.json()
                if (response.lifeCircle) {
                    this.rawLifeCycleData = response.lifeCircle.map(t => new Date(t))
                    this.updateChart()
                } else {
                    this.$message.error('未获取到新闻类别生命周期数据')
                }
            } catch (error) {
                console.error(error)
                this.$message.error('获取数据失败')
            }
        },

        updateChart() {
            if (!this.rawLifeCycleData.length) {
                this.chartInstance.setOption({
                    xAxis: { data: [] },
                    series: [{ data: [] }]
                })
                return
            }

            // 计算开始结束时间
            const sortedData = this.rawLifeCycleData.sort((a, b) => a - b)
            const startTime = sortedData[0]
            const endTime = sortedData[sortedData.length - 1]

            // 计算时间段
            const timeGrain = this.timeGranularity * 60 * 1000 // ms
            const timeSlots = []
            const counts = []
            let current = new Date(Math.floor(startTime.getTime() / timeGrain) * timeGrain)

            while (current <= endTime) {
                timeSlots.push(this.formatTime(current))
                counts.push(0)
                current = new Date(current.getTime() + timeGrain)
            }

            // 统计数量
            this.rawLifeCycleData.forEach(item => {
                const index = Math.floor((item.getTime() - startTime.getTime()) / timeGrain)
                if (index >= 0 && index < counts.length) {
                    counts[index]++
                }
            })

            // 更新图表
            this.chartInstance.setOption({
                xAxis: { data: timeSlots },
                series: [{ data: counts }]
            })
        },

        formatTime(date) {
            const h = String(date.getHours()).padStart(2, '0')
            const m = String(date.getMinutes()).padStart(2, '0')
            return `${h}:${m}`
        }
    }
}
</script>

<style>
.newsLifeCycle-box {
    padding: 10px;
}

.chart-wrapper {
    padding: 20px;
}
</style>
