<template>
    <div class="news-stats-container">
        <!-- 头部 -->
        <div class="header">
            <div>
                <h1>新闻多维统计查询</h1>
                <p>支持按时间、主题、长度、多用户组合灵活查询，全面分析新闻趋势</p>
            </div>
            <div class="stats-icon">
                <i class="el-icon-data-analysis"></i>
            </div>
        </div>

        <!-- 查询面板 -->
        <div class="search-panel">
            <el-form ref="form" :model="formData" label-position="top">
                <!-- 时间 -->
                <div class="filter-row">
                    <div class="filter-group half">
                        <div class="filter-label">时间范围</div>
                        <el-date-picker v-model="formData.dateRange" type="datetimerange" range-separator="至"
                            start-placeholder="开始时间" end-placeholder="结束时间" style="width:80%" />
                    </div>
                </div>
                <!-- 标题长度、内容长度范围 -->
                <div class="filter-row">
                    <div class="filter-group half">
                        <div class="filter-label">标题长度范围</div>
                        <div class="inline-input-group">
                            <el-input v-model="formData.titleMin" placeholder="最小" type="number" size="small"
                                style="width: 500px;" />
                            <div class="inline-separator">-</div>

                        </div>
                        <el-input v-model="formData.titleMax" placeholder="最大" type="number" size="small"
                            style="width: 500px; margin-top: 10px" />
                    </div>
                    <div class="filter-group half">
                        <div class="filter-label">内容长度范围</div>
                        <div class="inline-input-group">
                            <el-input v-model="formData.contentMin" placeholder="最小" type="number" size="small"
                                style="width: 500px; " />
                            <div class="inline-separator">-</div>

                        </div>
                        <el-input v-model="formData.contentMax" placeholder="最大" type="number" size="small"
                            style="width: 500px; margin-top: 10px" />
                    </div>
                </div>

                <div class="filter-row">
                    <!-- 新闻类别 -->
                    <div class="filter-group" style="flex: 1; margin-right: 16px;">
                        <div class="filter-label">新闻类别</div>
                        <div v-for="(topic, index) in formData.topics" :key="'topic-' + index" class="dynamic-item">
                            <el-input v-model="formData.topics[index]" placeholder="请输入类别"
                                style="width: 300px; margin-right: 8px;" clearable />
                            <el-button type="danger" icon="el-icon-close" size="mini" circle
                                @click="removeTopic(index)" />
                        </div>
                        <el-button type="primary" plain icon="el-icon-plus" size="mini"
                            @click="addTopic">添加类别</el-button>
                    </div>

                    <!-- 多用户ID -->
                    <div class="filter-group" style="flex: 1;">
                        <div class="filter-label">多用户ID</div>
                        <div v-for="(item, index) in formData.userIds" :key="'user-' + index" class="dynamic-item">
                            <el-input v-model="formData.userIds[index]" placeholder="输入用户ID"
                                style="width: 300px; margin-right: 8px;" clearable />
                            <el-button type="danger" icon="el-icon-minus" size="mini" @click="removeUserId(index)" />
                        </div>
                        <el-button type="primary" plain icon="el-icon-plus" size="mini"
                            @click="addUserId">添加用户</el-button>
                    </div>
                </div>


                <!-- 按钮 -->
                <div class="action-buttons">
                    <el-button type="primary" icon="el-icon-search" @click="searchData"
                        style="font-size: 18px; padding: 9px 18px;">查询</el-button>
                    <el-button icon="el-icon-refresh" @click="resetForm"
                        style="font-size: 18px; padding: 9px 18px;">重置</el-button>
                </div>
            </el-form>
        </div>
        <!-- 查询结果表格 -->
        <div class="table-wrapper">

            <el-table :data="records" border stripe class="table-list"
                style="word-break: break-word; min-width: 1000px;">

                <el-table-column prop="user_id" label="用户ID" header-align="center" align="center"
                    width="180"></el-table-column>

                <el-table-column prop="news_id" label="新闻ID" header-align="center" align="center"
                    width="180"></el-table-column>

                <el-table-column prop="start_time" label="开始时间" header-align="center" align="center"
                    width="200"></el-table-column>

                <el-table-column prop="duration" label="阅读时长(s)" header-align="center" align="center"
                    width="180"></el-table-column>

                <el-table-column prop="category" label="新闻类别" header-align="center" align="center"
                    width="180"></el-table-column>

                <el-table-column prop="topic" label="新闻主题" header-align="center" align="center"
                    width="180"></el-table-column>

                <el-table-column prop="title_len" label="标题长度" header-align="center" align="center"
                    width="150"></el-table-column>

                <el-table-column prop="body_len" label="正文长度" header-align="center" align="center"
                    width="150"></el-table-column>

            </el-table>

        </div>
        <div class="footer">© 2025 Tongji University PENS Analysis系统</div>
    </div>
</template>

<script>

module.exports = {
    name: "NewsStats",
    data() {
        return {
            formData: {
                dateRange: null,
                topics: [],
                titleMin: null,
                titleMax: null,
                contentMin: null,
                contentMax: null,
                userIds: []
            },
            records: []
        };
    },
    mounted() {
        //this.initMockRecords();
    },
    methods: {
        initMockRecords() {
            this.records = [
                {
                    user_id: "U001",
                    news_id: "N1001",
                    start_time: "2024-06-10 10:00:00",
                    duration: 35,
                    category: "体育",
                    topic: "NBA总决赛",
                    title_len: 20,
                    body_len: 3000
                },
                {
                    user_id: "U002",
                    news_id: "N1002",
                    start_time: "2024-06-10 11:20:00",
                    duration: 50,
                    category: "科技",
                    topic: "AI大模型",
                    title_len: 15,
                    body_len: 2800
                },
                {
                    user_id: "U003",
                    news_id: "N1003",
                    start_time: "2024-06-11 08:45:00",
                    duration: 60,
                    category: "娱乐",
                    topic: "电影节红毯",
                    title_len: 18,
                    body_len: 4500
                },

            ];
        },
        searchData() {
            const requestBody = {
                time: this.formData.dateRange || [],
                category: this.formData.topics,
                title_len: [this.formData.titleMin, this.formData.titleMax],
                body_len: [this.formData.contentMin, this.formData.contentMax],
                user_id: this.formData.userIds,
            };

            console.log("发送查询请求：", requestBody);

            newsApi.getRecordData(requestBody).then(response => {
                this.records = response.records;
            });
        },
        resetForm() {
            this.formData = {
                dateRange: null,
                topics: [],
                titleMin: null,
                titleMax: null,
                contentMin: null,
                contentMax: null,
                userIds: []
            };
        },
        addTopic() {
            this.formData.topics.push("");
        },
        removeTopic(index) {
            this.formData.topics.splice(index, 1);
        },
        addUserId() {
            this.formData.userIds.push("");
        },
        removeUserId(index) {
            this.formData.userIds.splice(index, 1);
        }
    }
};
</script>

<style scoped>
/* 你原来的 CSS 直接复制到这里 */

* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: 'Helvetica Neue', Arial, sans-serif;
}

body {
    background: linear-gradient(135deg, #f5f7fa 0%, #e4edf5 100%);
    min-height: 100vh;
    padding: 20px;
}

.record-table {
    overflow-x: auto;
    overflow-y: auto;
    max-height: 450px;
    border: 1px solid #eee;
    border-radius: 4px;
}



.inline-separator {
    padding: 0 4px;
}

.news-stats-container {
    max-width: 1400px;
    margin: 0 auto;
    background: white;
    border-radius: 12px;
    box-shadow: 0 8px 30px rgba(0, 0, 0, 0.12);
    overflow: hidden;
}

.header {
    background: linear-gradient(120deg, #1e88e5, #0d47a1);
    color: white;
    padding: 25px 30px;
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.header h1 {
    font-size: 28px;
    font-weight: 600;
    letter-spacing: 0.5px;
}

.header p {
    margin-top: 8px;
    opacity: 0.85;
    font-size: 15px;
    max-width: 700px;
}

.stats-icon {
    font-size: 48px;
    margin-left: 20px;
    opacity: 0.8;
}

.search-panel {
    padding: 25px 30px;
    background-color: #fafbfc;
    border-bottom: 1px solid #eaeef5;
}

.filter-row {
    display: flex;
    flex-wrap: wrap;
    margin: 0 -10px 15px;
}

.filter-group {
    flex: 1;
    min-width: 300px;
    padding: 0 10px;
    margin-bottom: 15px;
}

.filter-label {
    display: block;
    margin-bottom: 8px;
    font-weight: 600;
    color: #2c3e50;
    font-size: 14px;
}

.action-buttons {
    display: flex;
    justify-content: flex-end;
    padding-top: 10px;
    margin-top: 10px;
    border-top: 1px dashed #e0e6ed;
}

.chart-container {
    padding: 25px 30px;
}

.results-summary {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
    gap: 20px;
    margin-bottom: 30px;
}

.summary-card {
    background: white;
    border-radius: 10px;
    padding: 20px;
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.05);
    text-align: center;
    transition: all 0.3s ease;
    border-top: 4px solid #1e88e5;
}

.summary-card:hover {
    transform: translateY(-5px);
    box-shadow: 0 6px 16px rgba(0, 0, 0, 0.1);
}

.summary-card.blue {
    border-top-color: #1e88e5;
}

.summary-card.green {
    border-top-color: #43a047;
}

.summary-value {
    font-size: 32px;
    font-weight: 700;
    color: #2c3e50;
    margin: 10px 0;
}

.summary-label {
    font-size: 14px;
    color: #7f8c8d;
}

.footer {
    text-align: center;
    padding: 20px;
    color: #7f8c8d;
    font-size: 14px;
    border-top: 1px solid #eee;
}

@media (max-width: 768px) {
    .filter-group {
        min-width: 100%;
    }

    .header {
        flex-direction: column;
        text-align: center;
    }

    .stats-icon {
        margin: 15px 0 0;
    }
}
</style>
