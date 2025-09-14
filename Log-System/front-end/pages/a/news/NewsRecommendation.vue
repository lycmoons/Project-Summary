<template>
    <div class="hot-news-container">
        <div class="header">
            <div>
                <h1>用户新闻推荐</h1>
                <p>根据用户ID查询用户偏好新闻</p>
            </div>
            <div class="stats-icon">
                <i class="el-icon-user"></i>
            </div>
        </div>

        <div class="data-panel">
            <el-form label-position="top">
                <div class="filter-row">
                    <div class="filter-group">
                        <div class="filter-label">用户ID</div>
                        <el-input v-model="userId" placeholder="请输入用户ID" style="width: 300px;" />
                    </div>
                    <div class="filter-group">
                        <div class="filter-label">新闻类别</div>
                        <el-input v-model="category" class="input-style" disabled style="width: 300px;" />
                    </div>
                    <div class="filter-group">
                        <div class="filter-label">新闻主题</div>
                        <el-input v-model="topic" class="input-style" disabled style="width: 300px;" />
                    </div>
                </div>
            </el-form>
        </div>

        <div class="action-buttons">
            <el-button type="primary" icon="el-icon-search" @click="fetchHotNewsByUser">推荐</el-button>
            <!-- <el-button type="info" icon="el-icon-refresh" @click="fetchHotNews">刷新</el-button> -->
        </div>

        <div class="table-wrapper">
            <el-table :data="news" border stripe style="min-width: 1000px;">
                <el-table-column prop="id" label="新闻ID" align="center" width="250"></el-table-column>
                <el-table-column prop="category" label="类别" align="center" width="250"></el-table-column>
                <el-table-column prop="topic" label="主题" align="center" width="270"></el-table-column>
                <el-table-column prop="title_len" label="标题长度" align="center" width="270"></el-table-column>
                <el-table-column prop="body_len" label="正文长度" align="center" width="300"></el-table-column>
            </el-table>
        </div>

        <div class="footer">© 2025 Tongji University HotNews Dashboard</div>
    </div>
</template>

<script>

module.exports = {
    name: "HotNewsByUser",
    data() {
        return {
            userId: "",
            category: "",
            topic: "",
            news: [],
        };
    },
    mounted() {
        this.fetchHotNews(); // 默认加载无参全量
    },
    methods: {
        fetchHotNews() {
            hotNewsApi.getHotNews().then((res) => {
                this.category = res.category;
                this.topic = res.topic;
                this.news = res.news;
            });
        },
        fetchHotNewsByUser() {
            if (!this.userId) {
                this.$message.warning("请先输入用户ID！");
                return;
            }
            newsApi.getNewsRecommendation(this.userId).then((res) => {
                this.category = res.category;
                this.topic = res.topic;
                this.news = res.news;
            });
        },
    },
};
</script>

<style scoped>
.input-style .el-input__inner {
    font-size: 16px;
    font-weight: bold;
    font-family: "Microsoft YaHei", "PingFang SC", "Helvetica Neue", Helvetica, Arial, sans-serif;
    color: #333;
    background-color: #f9f9f9;
    border-radius: 4px;
}

.hot-news-container {
    max-width: 1400px;
    margin: 0 auto;
    background: white;
    border-radius: 12px;
    box-shadow: 0 8px 30px rgba(0, 0, 0, 0.12);
    overflow: hidden;
}

.header {
    background: linear-gradient(120deg, #35e58d, #1cb790);
    color: white;
    padding: 25px 30px;
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.header h1 {
    font-size: 28px;
    font-weight: 600;
}

.header p {
    margin-top: 8px;
    opacity: 0.85;
    font-size: 15px;
}

.stats-icon {
    font-size: 48px;
    margin-left: 20px;
    opacity: 0.8;
}

.data-panel {
    padding: 25px 30px;
    background-color: #fafbfc;
    border-bottom: 1px solid #eaeef5;
}

.filter-row {
    display: flex;
    flex-wrap: wrap;
    gap: 20px;
    margin-bottom: 20px;
}

.filter-group {
    flex: 1;
    min-width: 250px;
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
    padding: 10px 30px;
    border-bottom: 1px dashed #e0e6ed;
}

.table-wrapper {
    padding: 25px 30px;
}

.footer {
    text-align: center;
    padding: 20px;
    color: #7f8c8d;
    font-size: 14px;
    border-top: 1px solid #eee;
}
</style>
