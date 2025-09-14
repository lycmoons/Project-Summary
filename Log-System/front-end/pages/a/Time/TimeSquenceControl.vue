<template>
    <div class="time-control-container">
        <div class="header">
            <div>
                <h1>时序控制面板</h1>
                <p>操作当前系统时序</p>
            </div>
            <div class="stats-icon">
                <i class="el-icon-time"></i>
            </div>
        </div>

        <div class="action-buttons">
            <el-button type="primary" @click="getCurrentTime">获取当前时序</el-button>
            <el-button type="success" @click="addTime">时序加一天</el-button>
            <el-button type="danger" @click="resetTime">重置时序</el-button>
        </div>

        <div class="time-display" v-if="curDate">
            当前时序日期：<strong>{{ curDate }}</strong>
        </div>

        <div class="footer">© 2025 Tongji University TimeControl Dashboard</div>
    </div>
</template>

<script>


module.exports = {
    name: "TimeControl",
    data() {
        return {
            curDate: ""
        };
    },
    methods: {
        getCurrentTime() {
            timeSquenceApi.getCurrentTime().then(res => {
                this.curDate = res.cur_date;
            });
        },
        addTime() {
            timeSquenceApi.addTime().then(res => {
                if (res.status) {
                    this.$message.success("时序已成功增加一天！");
                    this.getCurrentTime();
                } else {
                    this.$message.error("操作失败！");
                }
            });
        },
        resetTime() {
            timeSquenceApi.resetTime().then(res => {
                if (res.status) {
                    this.$message.success("时序已重置！");
                    this.getCurrentTime();
                } else {
                    this.$message.error("重置失败！");
                }
            });
        }
    }
};
</script>

<style scoped>
.time-control-container {
    max-width: 800px;
    margin: 0 auto;
    background: white;
    border-radius: 12px;
    box-shadow: 0 8px 30px rgba(0, 0, 0, 0.12);
    overflow: hidden;
    padding-bottom: 30px;
}

.header {
    background: linear-gradient(120deg, #3949ab, #1a237e);
    color: white;
    padding: 25px 30px;
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.header h1 {
    font-size: 26px;
    font-weight: 600;
}

.header p {
    margin-top: 8px;
    opacity: 0.85;
    font-size: 14px;
}

.stats-icon {
    font-size: 42px;
    margin-left: 20px;
    opacity: 0.8;
}

.action-buttons {
    display: flex;
    justify-content: space-around;
    padding: 30px 20px 10px;
}

.time-display {
    text-align: center;
    font-size: 18px;
    padding: 30px 0 20px;
    color: #2c3e50;
}

.footer {
    text-align: center;
    padding: 20px;
    color: #7f8c8d;
    font-size: 14px;
    border-top: 1px solid #eee;
}
</style>
