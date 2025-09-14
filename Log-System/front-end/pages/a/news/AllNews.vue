<template>
    <div class="newslist-box">

        <el-tabs v-model="activeTab" type="border-card">
            <el-tab-pane label="新闻列表" name="newsList">

                <div class="table-wrapper">

                    <el-table :data="newsList" border stripe class="table-list" style="word-break: break-word;">

                        <el-table-column prop="id" label="新闻ID" header-align="center" align="center"
                            width="150"></el-table-column>

                        <el-table-column prop="title" label="新闻标题" header-align="center" align="left"
                            min-width="180"></el-table-column>

                        <el-table-column prop="content" label="新闻正文" header-align="center" align="left" min-width="250">
                            <template slot-scope="scope">
                                <el-tooltip class="item" effect="dark" :content="scope.row.content" placement="top">
                                    <span>{{ scope.row.content.length > 50 ? scope.row.content.slice(0, 50) + '...' :
                                        scope.row.content }}</span>
                                </el-tooltip>
                            </template>
                        </el-table-column>

                        <el-table-column prop="category" label="新闻类别" header-align="center" align="center"
                            width="150"></el-table-column>

                    </el-table>

                </div>

            </el-tab-pane>
        </el-tabs>

    </div>
</template>

<script>
module.exports = {
    data() {
        return {
            activeTab: "newsList",
            newsList: []
        }
    },
    mounted() {
        this.initNewsList();
    },
    methods: {
        initNewsList() {
            newsApi.list().then(response => {
                this.newsList = response;
            });
        },
        viewDetail(newsId) {
            const targetPath = "/newsMng/detail?newsId=" + newsId;
            this.$parent.$router.push({ path: targetPath });
        },
        deleteNews(newsId) {
            this.$confirm('确认删除该新闻吗？', '提示', {
                confirmButtonText: '确认删除',
                cancelButtonText: '再考虑一下',
                type: 'warning'
            }).then(() => {
                newsApi.delete(newsId).then(response => {
                    this.$message({
                        showClose: true,
                        message: '删除成功！',
                        type: 'success',
                        duration: 2000
                    });
                    this.initNewsList();
                });
            }).catch(() => { });
        }
    }
}
</script>

<style>
.newslist-box {
    display: flex;
    flex-direction: column;
    justify-content: flex-start;
    font-size: 16px;
}

.newslist-box .table-wrapper {
    padding: 10px 10px 20px 10px;
}

.newslist-box .table-list {
    width: 100%;
    font-size: 15px;
}

.newslist-box .search-area {
    margin-bottom: 20px;
}
</style>
