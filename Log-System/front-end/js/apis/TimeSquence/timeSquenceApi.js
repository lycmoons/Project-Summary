window.timeSquenceApi = {

    getCurrentTime: function () {   //获取当前时序
        return instance({
            url: '/cur/time',
            method: 'get',
        });
    },
    resetTime: function () {   //时序重置操作
        return instance({
            url: 'reset/time',
            method: 'get',
        });
    },
    addTime: function () {   //时序加一天操作
        return instance({
            url: 'add/time',
            method: 'get',
        });
    },

}

