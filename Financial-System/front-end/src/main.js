import { createApp } from 'vue'
import App from './App.vue'
import router from "@/router/index.js"
import 'element-plus/dist/index.css'
import ElementPlus from 'element-plus'
import axios from "axios"
import "@/assets/global.css"

axios.defaults.baseURL = 'http://127.0.0.1:8080'
axios.defaults.headers.post['Content-Type'] = 'application/json';
axios.defaults.withCredentials = false; // 如果没用cookie验证，设为false

const app = createApp(App)
app.use(ElementPlus)
app.use(router)
app.mount('#app')

