<template>
    <div class="logs-container">
        <div class="header">
            <div>
                <h1>系统日志总览</h1>
                <p>实时查看系统运行日志记录</p>
            </div>
            <div class="stats-icon">
                <i class="el-icon-document"></i>
            </div>
        </div>

        <div class="action-buttons">
            <el-button type="primary" icon="el-icon-refresh" @click="fetchLogs">获取所有日志</el-button>
        </div>

        <div class="table-wrapper">
            <el-table :data="logs" border stripe style="min-width: 1000px;">
                <el-table-column prop="id" label="日志ID" align="center" width="250"></el-table-column>
                <el-table-column prop="content" label="日志内容" align="center" width="500"></el-table-column>
                <el-table-column prop="start" label="开始时间" align="center" width="300"></el-table-column>
                <el-table-column prop="end" label="结束时间" align="center" width="290"></el-table-column>
            </el-table>
        </div>

        <div class="footer">© 2025 Tongji University Logs Dashboard</div>
    </div>
</template>

<script>

module.exports = {
    name: "AllLogs",
    data() {
        return {
            logs: [],
        };
    },
    mounted() {
        this.fetchLogs();
    },
    methods: {
        fetchLogs() {
            logApi.getAllLogs().then((res) => {
                this.logs = res.logs;
            });
        },
    },
};
</script>

<style scoped>
.logs-container {
    max-width: 1400px;
    margin: 0 auto;
    background: white;
    border-radius: 12px;
    box-shadow: 0 8px 30px rgba(0, 0, 0, 0.12);
    overflow: hidden;
}

.header {
    background: linear-gradient(120deg, #1976d2, #0d47a1);
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
