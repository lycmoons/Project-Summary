window.newsApi = {

    getNewsLifeCycleData: function (id) {
        return instance({
            url: '/lifeCircle/byNews',
            method: 'post',
            data: { id: id }
        })
    },
    getCategoryLifeCycleData: function (category) {
        return instance({
            url: '/lifeCircle/byCategory',
            method: 'post',
            data: { category: category }
        })
    },
    getUserInterestData: function (id) { //获取某个用户兴趣变化
        return instance({
            url: '/hobby',
            method: 'post',
            data: { id: id }
        })
    },
    getRecordData: function (params) {   //根据组合条件获取新闻曝光日志
        return instance({
            url: '/condition/check',
            method: 'post',
            data: params
        });
    },
    getHotNews: function () {   //根据组合条件获取新闻曝光日志
        return instance({
            url: '/popularity',
            method: 'get',
        });
    },
    getNewsRecommendation: function (id) {   //根据组合条件获取新闻曝光日志
        return instance({
            url: 'popularity/byUser',
            method: 'post',
            data: { id: id }
        });
    },


}
