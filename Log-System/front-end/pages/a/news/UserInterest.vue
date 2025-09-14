<template>
    <div class="user-interest-app">
        <header class="app-header">
            <h1>用户兴趣分析</h1>
            <p>输入用户ID进行搜索，下方展示该用户每日点击的新闻堆叠柱状图</p>
        </header>

        <section class="dashboard-container">
            <div class="search-section">
                <el-input v-model="searchKeyword" placeholder="请输入用户ID" clearable @keyup.enter.native="handleSearch"
                    class="search-box">
                    <template #append>
                        <el-button icon="el-icon-search" @click="handleSearch" class="search-btn"></el-button>
                    </template>
                </el-input>
            </div>



            <div class="chart-container" v-if="chartData && chartData.date.length">
                <div class="chart-title">用户兴趣柱状图</div>
                <div ref="chart" class="chart-wrapper"></div>
            </div>
        </section>
    </div>
</template>

<script>
const echarts = require('echarts')

module.exports = {
    data() {
        return {
            searchKeyword: "",
            userInfo: null,
            chartData: null,  // 改成对象，不再是数组
            chartInstance: null
        }
    },
    computed: {
        userInitial() {
            if (!this.userInfo || !this.userInfo.name) return ''
            return this.userInfo.name.charAt(0).toUpperCase()
        }
    },
    watch: {
        chartData() {
            this.$nextTick(() => {
                this.drawChart()
            })
        }
    },
    methods: {
        handleSearch() {
            if (!this.searchKeyword) {
                this.$message.warning('请输入用户ID')
                return
            }

            // 异步请求用户数据
            this.fetchUserData(this.searchKeyword)
        },
        handleClear() {
            this.searchKeyword = ""
            this.chartData = []
            if (this.chartInstance) {
                this.chartInstance.clear()
            }
        },
        fetchUserData(userId) {

            // 这里是模拟数据
            // 模拟后端返回格式
            // const mockResponse = {
            //     date: ["2024-06-10", "2024-06-11"],
            //     data: [
            //         { category: ["体育", "娱乐", "美食"], sum: [120, 80, 10] },
            //         { category: ["体育", "娱乐"], sum: [140, 60] }
            //     ]
            // }

            // setTimeout(() => {
            //     this.userInfo = { id: userId, name: '张三' }  // 只保留id和name，其他字段不重要了
            //     this.chartData = mockResponse  // 直接赋值
            // }, 200)
            newsApi.getUserInterestData(userId).then(response => {
                const res = response

                // 更新用户ID信息就行，名字不要了
                //this.userInfo = { id: userId }

                // 直接保存 chartData
                this.chartData = res
            }).catch(error => {
                console.error('获取用户兴趣数据失败:', error)
                this.$message.error('数据请求失败，请稍后重试')
            })
        },
        drawChart() {
            if (!this.chartInstance) {
                this.chartInstance = echarts.init(this.$refs.chart)
            }

            const { date, data } = this.chartData

            // 获取所有类别，确保全量唯一
            const categories = [...new Set(data.flatMap(item => item.category))]

            // 每个类别一条 series
            const series = categories.map(cat => {
                return {
                    name: cat,
                    type: 'bar',
                    stack: 'total',  // 堆叠柱状图
                    emphasis: { focus: 'series' },
                    data: date.map((_, dateIndex) => {
                        const catIndex = data[dateIndex].category.indexOf(cat)
                        return catIndex !== -1 ? data[dateIndex].sum[catIndex] : 0
                    })
                }
            })

            const option = {
                tooltip: { trigger: 'axis', axisPointer: { type: 'shadow' } },
                legend: { top: 10 },
                grid: { top: 60, left: '3%', right: '4%', bottom: '3%', containLabel: true },
                xAxis: {
                    type: 'category',
                    data: date
                },
                yAxis: {
                    type: 'value',
                    name: '点击次数'
                },
                series: series
            }

            this.chartInstance.setOption(option)
        }

    }
}
</script>

<style scoped>
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: 'Helvetica Neue', Arial, sans-serif;
}

.search-btn .el-icon-search {
    position: relative;
    top: 18px;
    /* 往下移动2px，按需调整 */
}

.user-interest-app {
    max-width: 1200px;
    margin: 0 auto;
}

.app-header {
    text-align: center;
    margin-bottom: 30px;
    padding: 20px;
    background: white;
    border-radius: 12px;
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.08);
}

.app-header h1 {
    color: #2c3e50;
    font-size: 2.5rem;
    margin-bottom: 10px;
    background: linear-gradient(90deg, #3498db, #2c3e50);
    -webkit-background-clip: text;
    background-clip: text;
    color: transparent;
}

.app-header p {
    color: #7f8c8d;
    font-size: 1.1rem;
    max-width: 700px;
    margin: 0 auto;
    line-height: 1.6;
}

.dashboard-container {
    background: white;
    border-radius: 12px;
    box-shadow: 0 6px 30px rgba(0, 0, 0, 0.1);
    overflow: hidden;
    padding: 25px;
}

.search-section {
    display: flex;
    justify-content: center;
    margin-bottom: 30px;
    position: relative;
}

.search-box {
    width: 100%;
    max-width: 600px;
    display: flex;
    box-shadow: 0 4px 12px rgba(52, 152, 219, 0.2);
    border-radius: 30px;
    overflow: hidden;
}

.search-box .el-input {
    flex: 1;
}

.search-box .el-input__inner {
    height: 50px;
    border: none;
    padding: 0 25px;
    font-size: 16px;
    border-radius: 30px 0 0 30px;
}

.search-box .el-input-group__append {
    background: #3498db;
    border: none;
    border-radius: 0 30px 30px 0;
    padding: 0 25px;
    cursor: pointer;
    transition: all 0.3s ease;
}

.search-box .el-input-group__append:hover {
    background: #2980b9;
}

.search-box .el-button {
    color: white;
    font-size: 18px;
}

.chart-container {
    background: #fff;
    border-radius: 10px;
    box-shadow: 0 4px 15px rgba(0, 0, 0, 0.05);
    padding: 20px;
    transition: all 0.3s ease;
}

.chart-container:hover {
    transform: translateY(-5px);
    box-shadow: 0 8px 25px rgba(0, 0, 0, 0.1);
}

.chart-title {
    text-align: center;
    font-size: 1.4rem;
    color: #2c3e50;
    margin-bottom: 20px;
    padding-bottom: 15px;
    border-bottom: 2px solid #f0f5f9;
    position: relative;
}

.chart-title::after {
    content: '';
    position: absolute;
    bottom: -2px;
    left: 50%;
    transform: translateX(-50%);
    width: 80px;
    height: 3px;
    background: linear-gradient(90deg, #3498db, #2ecc71);
    border-radius: 3px;
}

.chart-wrapper {
    height: 400px;
    width: 100%;
}

.user-info {
    display: flex;
    align-items: center;
    justify-content: center;
    margin: 20px 0;
    padding: 15px;
    background: #f8fafc;
    border-radius: 10px;
    box-shadow: 0 2px 10px rgba(0, 0, 0, 0.03);
}

.avatar {
    width: 70px;
    height: 70px;
    border-radius: 50%;
    background: linear-gradient(135deg, #3498db, #2ecc71);
    display: flex;
    align-items: center;
    justify-content: center;
    color: white;
    font-size: 28px;
    font-weight: bold;
    margin-right: 20px;
}

.user-details {
    flex: 1;
}

.user-name {
    font-size: 1.5rem;
    color: #2c3e50;
    margin-bottom: 5px;
}

.user-stats {
    display: flex;
    gap: 20px;
}

.stat-item {
    text-align: center;
    padding: 10px 15px;
    background: white;
    border-radius: 8px;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
}

.stat-value {
    font-size: 1.3rem;
    font-weight: bold;
    color: #3498db;
}
</style>
