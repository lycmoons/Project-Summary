<script setup>
import { reactive, ref } from "vue";
import { User, Message, Lock, Clock } from "@element-plus/icons-vue"
import router from "@/router/index.js";

import { validateCode, validateEmail, validatePassword, validateUsername } from "@/validate/index.js";
import axios from "axios";
import { ElMessage } from "element-plus";

const formRef = ref()
const form = reactive({
  username: '',
  password: '',
  password_repeat: '',
  email: '',
  code: ''
})

function askForCode() {
  axios.post('/User/SendCode', {
    email: form.email
  }).then(({ data }) => {
    if (data.code === 200) {  // 服务器处理成功
      ElMessage.success('已发送验证码至指定邮箱')
    }
    else {   // 服务器处理失败
      console.warn(`请求地址：/User/SendCode，状态码：${data.code}，错误信息：${data.message}`)
      ElMessage.warning(data.message)
    }
  }).catch(err => {  // 连接失败
    console.warn(err)
    ElMessage.warning('发生了一些错误，请联系管理员')
  })
}

function userRegister() {
  axios.post('/User/AddUser', {
    username: form.username,
    password: form.password,
    email: form.email,
    code: form.code
  }).then(({ data }) => {
    if (data.code === 200) {  // 服务器处理成功
      ElMessage.success('用户注册成功')
      router.push('/')
    }
    else {   // 服务器处理失败
      console.warn(`请求地址：/User/SendCode，状态码：${data.code}，错误信息：${data.message}`)
      ElMessage.warning(data.message)
    }
  }).catch(err => {  // 连接失败
    console.warn(err)
    ElMessage.warning('发生了一些错误，请联系管理员')
  })
}

const validatePassword_repeat = (rule, value, callback) => {
  if (value === '') {
    callback(new Error('请再次输入密码进行确认'))
  }
  else if (value !== form.password) {
    callback(new Error('两次输入的密码不一致'))
  }
  else {
    callback()
  }
}

const rule = {
  username: [
    { validator: validateUsername, trigger: ['blur', 'change'] },
  ],
  password: [
    { validator: validatePassword, trigger: ['blur', 'change'] },
  ],
  password_repeat: [
    { validator: validatePassword_repeat, trigger: ['blur', 'change'] },
  ],
  email: [
    { validator: validateEmail, trigger: ['blur', 'change'] },
  ],
  code: [
    { validator: validateCode, trigger: ['blur', 'change'] },
  ]
}

</script>

<template>
  <div style="text-align: center;margin: 0 20px">
    <div style="margin-top: 0">
      <div style="font-size: 25px;font-weight: bold;color: white">注册</div>
    </div>
    <div style="margin-top: 50px">
      <el-form :model="form" :rules="rule" ref="formRef">
        <el-form-item prop="username">
          <el-input v-model="form.username" maxlength="10" type="text" placeholder="用户名">
            <template #prefix>
              <el-icon>
                <User />
              </el-icon>
            </template>
          </el-input>
        </el-form-item>

        <el-form-item prop="password">
          <el-input v-model="form.password" maxlength="20" type="password" placeholder="密码">
            <template #prefix>
              <el-icon>
                <Lock />
              </el-icon>
            </template>
          </el-input>
        </el-form-item>

        <el-form-item prop="password_repeat">
          <el-input v-model="form.password_repeat" maxlength="20" type="password" placeholder="再次输入确认密码">
            <template #prefix>
              <el-icon>
                <Lock />
              </el-icon>
            </template>
          </el-input>
        </el-form-item>

        <el-form-item prop="email">
          <el-input v-model="form.email" type="text" placeholder="电子邮件地址">
            <template #prefix>
              <el-icon>
                <Message />
              </el-icon>
            </template>
          </el-input>
        </el-form-item>

        <el-form-item prop="code">
          <el-row gutter="10" style="width: 100%">
            <el-col :span="17">
              <el-input v-model="form.code" maxlength="6" type="text" placeholder="请输入验证码">
                <template #prefix>
                  <el-icon>
                    <Clock />
                  </el-icon>
                </template>
              </el-input>
            </el-col>

            <el-col :span="5">
              <el-button @click="askForCode" class="askCode">获取验证码</el-button>
            </el-col>
          </el-row>
        </el-form-item>
      </el-form>
    </div>
    <div style="margin-top: 50px">
      <el-button @click="userRegister" class="register" style="width: 250px">立即注册</el-button>
    </div>
    <div style="margin-top: 20px">
      <span style="font-size: 14px;line-height: 15px;color: white">已有账号？</span>
      <el-link style="translate: 0 -1px;color: white" @click="router.push('/')">立即登录</el-link>
    </div>
  </div>
</template>

<style scoped>
.register {
  border-radius: 50px;
  background: #946438;
  box-shadow: inset 24px 24px 35px #49311b,
    inset -24px -24px 35px #df9755;
  color: white;
}

::v-deep(.register:hover) {
  border-radius: 50px;
  background: #684627;
  box-shadow: inset 24px 24px 35px #332213,
    inset -24px -24px 35px #9d6a3b;
}

.askCode {
  border-radius: 50px;
  background: #669438;
  box-shadow: inset 24px 24px 35px #32491b,
    inset -24px -24px 35px #9adf55;
  color: white;
}

::v-deep(.askCode:hover) {
  border-radius: 50px;
  background: #4d6f2a;
  box-shadow: inset 24px 24px 35px #263615,
    inset -24px -24px 35px #74a83f;
}
</style>