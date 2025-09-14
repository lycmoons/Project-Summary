import {createRouter, createWebHistory} from "vue-router"

const router = createRouter({
    history: createWebHistory(import.meta.env.BASE_URL),
    routes: [
        // 登录界面
        {
            path:'/',
            name: 'welcome',
            component: () => import('@/views/WelcomeView.vue'),
            children: [
                {
                    path: '',
                    name: 'welcome-login',
                    component: () => import('@/views/welcome/LoginPage.vue')
                },
                {
                    path: 'register',
                    name: 'welcome-register',
                    component: () => import('@/views/welcome/RegisterPage.vue')
                },
                {
                    path: 'reset',
                    name: 'welcome-reset',
                    component: () => import('@/views/welcome/ResetPage.vue')
                }
            ]
        },

        // 主界面
        {
            path: '/index',
            name: 'index',
            component: () => import('@/views/IndexView.vue'),
            children: [
                // 新闻快讯
                {
                    path: '',
                    name: 'index-news',
                    component: () => import('@/views/index/NewsPage.vue')
                },

                // 行业信息
                {
                    path: 'industry',
                    name: 'index-industry',
                    component: () => import('@/views/index/IndustryPage.vue')
                },

                // 企业信用评估
                {
                    path: 'credit',
                    name: 'index-credit',
                    component: () => import('@/views/index/CreditPage.vue')
                },

                // 金融风险预测
                {
                    path: 'risk',
                    name: 'index-risk',
                    component: () => import('@/views/index/RiskPage.vue')
                },

                // 图表分析
                {
                    path: 'chart',
                    name: 'index-chart',
                    component: () => import('@/views/index/ChartPage.vue')
                }
            ]
        },


        // 其他组建界面
        {
            path: '/company/:uid',
            name: 'company',
            component: () => import('@/components/CompanyDetail.vue')
        },
        {
            path: '/news-content/:id',
            name: 'news-content',
            component: () => import('@/components/NewsDetail.vue')
        }
    ]
})

export default router