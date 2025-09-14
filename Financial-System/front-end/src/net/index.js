import {ElMessage} from "element-plus";
import router from "@/router/index.js";
import axios from "axios";

// token 存储函数
export function saveToken(token, remember){
    const obj = { token: token }
    const str = JSON.stringify(obj)
    if(remember){
        localStorage.setItem('tokenKey', str)
    }
    else{
        sessionStorage.setItem('tokenKey', str)
    }
}

// 获取 token 函数
function getToken(){
    let str = localStorage.getItem('tokenKey')
    if(!str) str = sessionStorage.getItem('tokenKey')

    // 检查是否登录
    if(!str) return null
    const obj = JSON.parse(str)
    return obj.token
}

// 封装 token 请求头
export function getTokenHeader(){
    const token = getToken()
    if(!token) return null
    return { 'Authorization': `Bearer ${token}` }
}

// 带有 token 请求头的 GET 请求封装
export function get(url, success){
    const header = getTokenHeader()
    if(!header){
        // token 无效，不能发送本次请求
        ElMessage.warning('登录状态过期，请重新登录')
        router.push('/')
        return
    }

    axios.get(url, {
        headers: header
    }).then(({data}) => {
        if(data.code === 200){  // 服务器处理成功
            success(data.data)
        }
        else{   // 服务器处理失败
            console.warn(`请求地址：${url}，状态码：${data.code}，错误信息：${data.message}`)
            ElMessage.warning(data.message)
        }
    }).catch(err => {  // 连接失败
        console.warn(err)
        ElMessage.warning('发生了一些错误，请联系管理员')
    })
}

// 带有 token 请求头的 POST 请求封装
export function post(url, data, success){
    const header = getTokenHeader()
    if(!header){
        // token 无效，不能发送本次请求
        ElMessage.warning('登录状态过期，请重新登录')
        router.push('/')
        return
    }

    axios.post(url, data, {
        headers: header
    }).then(({data}) => {
        if(data.code === 200){  // 服务器处理成功
            success(data.data)
        }
        else{   // 服务器处理失败
            console.warn(`请求地址：${url}，状态码：${data.code}，错误信息：${data.message}`)
            ElMessage.warning(data.message)
        }
    }).catch(err => {  // 连接失败
        console.warn(err)
        ElMessage.warning('发生了一些错误，请联系管理员')
    })
}

// 发送 multipart/form-data 的 POST 请求
export function postMultipart(url, formData, success) {
    const token = getToken()
    if(!token) {
        // token 无效，不能发送本次请求
        ElMessage.warning('登录状态过期，请重新登录')
        router.push('/')
        return
    }
    axios.post(url, formData, {
        headers: {
            'Content-Type': 'multipart/form-data',
            'Authorization': `Bearer ${token}`
        }
    }).then(({data}) => {
        if(data.code === 200){  // 服务器处理成功
            success(data.data)
        }
        else{   // 服务器处理失败
            console.warn(`请求地址：${url}，状态码：${data.code}，错误信息：${data.message}`)
            ElMessage.warning(data.message)
        }
    }).catch(err => {  // 连接失败
        console.warn(err)
        ElMessage.warning('发生了一些错误，请联系管理员')
    })
}