window.logApi = {

    getAllLogs: function () {   //根据组合条件获取新闻曝光日志
        return instance({
            url: '/log',
            method: 'get',
        });
    },

}

