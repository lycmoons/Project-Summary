- 用户登陆 **POST**

- http://localhost:8080/User/Login

前端发送：

```json
{
    email:   // 用户名
    password:   // 密码
}
```

后端发送：

```json
{
    token:   // 用户令牌
}
```



- 请求邮箱验证码 **POST**
- http://localhost:8080/User/SendCode

前端发送：

```json
{
    email:   // 用户邮箱
}
```

后端发送：

```json
{
    null  // data 部分为null，但是 code 为 200，失败则为其它
}
```



- 用户注册 **POST**
- http://localhost:8080/User/AddUser

前端发送：

```json
{
    username:    // 用户名
    password:    // 密码
    email:       // 邮箱
    code:        // 验证码
}
```

后端发送：

```json
{
    null  // data 部分为null，但是 code 为 200，失败则为其它
}
```



- 用户修改密码 **POST**
- http://localhost:8080/User/ModifyPassword

前端发送：

```json
{
    password:   // 新密码
    email:      // 邮箱
    code:       // 验证码
}
```

后端发送：

```json
{
    null  // data 部分为null，但是 code 为 200，失败则为其它
}
```



- 获取所有公司的基本信息 **GET**
- http://localhost:8080/Company/GetAllCompaniesInfo

后端发送：

```json
{
    companies: [
        {
            uid:           // UID
            account:       // 户号
            name:          // 企业名称
            industry:      // 企业领域
            scale:         // 企业规模
            state:         // 登记状态
            score:         // 企查分
        },

        {
            // ...
        },

        // ...
    ]
}
```



- 获取某一个公司的所有信息 **POST **
- http://localhost:8080/Company/GetCompanyInfo

前端发送：

```json
{
    uid:   // UID
}
```

后端发送：

```json
{
    uid:                  // UID
    account:              // 户号
    name:                 // 企业名称
    industry:             // 企业领域
    scale:                // 企业规模
    state:                // 登记状态
    score:                // 企查分
    credit:               // 统一社会信用代码
    tag:                  // 企业标签
    date:                 // 成立日期
    ddl:                  // 经营期限
    qualification:        // 纳税人资质
    registerCapital:      // 注册资金
    realCapital:          // 实缴资金
    type:                 // 企业类型
    employeeScale:        // 人员规模
    insuredEmployee:      // 参保人数
    nationalStandard:     // 国标企业
    businessScope:        // 经营范围
}
```



- 获取所有新闻的基本信息 **GET**
- http://localhost:8080/News/GetAllNews

后端发送：

```json
{
    news: [
        {
            id:           // id
            photo_url:    // img_url
            title:        // title
            time:         // 计算updatetime与现在时间的差距，返回说明字符串如："1 hour ago"、"20 minutes ago"...
            type:         // rel_block_name
        },
        
        // ...
    ]
}
```



- 获取某一个新闻的具体内容 **POST **
- http://localhost:8080/News/GetNews 

前端发送：

```json
{
    id:    // id
}
```

后端发送：

```json
{
    id:             // id
    title:          // title
    time:           // 说明字符串
    content:        // content
    photo_url:      // img_url
}
```



- 获取所有行业的上市公司 **GET**
- http://localhost:8080/Company/GetListedCompanies

后端发送：

```json
{
    companies: [
        {
            uid:        // UID
            name:       // 公司名称
            industry:   // 公司领域
        },
        // ...
    ]
}
```



- 获取某个行业的图表所需数据 **POST **
- http://localhost:8080/Graph/GetIndustryGraphData

前端发送：

```json
{
    industry:    // 某个企业领域（行业）的名称
}
```

后端发送：

```json
{
    // 用于绘制柱状图的数据
    chart: {
        date: [],    // 对应这个行业的所有数据中date从小到大的排列
        volume: []   // 与上面date对应顺序的volume值（市值）
    },
    
    // 用于绘制表格的数据
    table: [
    	{
            date:         // 日期
            code:         // 行业代码
            open:         // 开盘价
            close:        // 收盘价
            turnOver:     // 成交量
            totalShare:   // 总股数
        },
        // ...
    ]
}
```



- 获取某个上市公司的图表所需数据 **POST **
- http://localhost:8080/Graph/GetListedCompanyGraphData

前端发送:

```json
{
    uid:    // 该上市公司对应的 UID
}
```

后端发送：

```json
{
    yearList: [],     // 从小到大的年份（字符串）
    profits: [],      // 对应年份该公司的归母净利润
    rate: [],         // 对应年份该公司的同比
    money: [],        // 对应年份该公司的总资产
    debt: [],         // 对应年份该公司的总负债
    debt_rate: []     // 对应年份该公司的负债率
}
```



- 获取与某个上市公司相关的新闻信息 **POST**
- http://localhost:8080/Company/GetCompanyNews

前端发送：

```json
{
    uid:    // 该上市公司对应的 UID
}
```

后端发送：

```json
{
    news: [
        {
            id:           // id
            photo_url:    // img_url6
            title:        // title
            time:         // 计算updatetime与现在时间的差距，返回说明字符串如："1 hour ago"、"20 minutes ago"...
            type:         // rel_block_name
        },
        
        // ...
    ]
}
```



- 获取某个上市公司股票市值的变化 **POST**
- http://localhost:8080/Company/GetStockChange

前端发送：

```json
{
    uid:    // 该上市公司对应的 UID
}
```

后端发送：

```json
{
    dateList: []     // 日期字符串列表
    value: []        // 对应日期的市值列表
}
```



- 获取基于决策树的企业信用等级评估结果 **POST**
- http://localhost:8080/Model/GetCredit

前端发送：

```json
{
    debt_rate:         // 资产负债率（%）
    interest:          // EBITDA利息数（倍）
    inventory_turn:    // 存货周转次数（次）
    flow_debt:         // 现金流动负债比率或EBITDA（%）
    cost:              // 成本费用利润率（%）
    owner_equity:      // 所有者权益合计（万元）
}
```

后端发送：

```json
{
    debt_rate: []
    interest: []
    inventory_turn: []
    flow_debt: []
    cost: []
    owner_equity: []
}
// 其中每一项都是一个有 6 个元素的数组
// 前 5 个元素分别是这一个指标在整个企业所在行业中的 最小值、Q1、中位数、Q2、最大值
// 第 6 个元素是这个企业这一指标的值
```



- 获取神经网络预测所需的模版CSV文件 **GET**
- http://localhost:8080/Model/GetTemplate

后端发送：

```json
{
    templateFile:     // 文件类型的数据
}
```



- 获取神经网络预测的结果 **POST**
- http://localhost:8080/Model/GetCnnPredict

前端发送：

```json
{
    targetFile:     // 文件类型数据
}
```

后端发送：

```json
{
    star:              // 星级，整数
}
```



- 获取财务风险评估结果 **POST**
- http://localhost:8080/Model/GetFinancialRisk

前端发送：

```json
{
    i1:     // 流动资产周转率
    i2:     // 应收账款周转率
    i3:     // 纳税信用等级
    i4:     // 1/财务费用率
    i5:     // 1/产权比率
    i6:     // 流动比率
    i7:     // 资产负债率
    i8:     // 利息保障倍数
    i9:     // 现金流量利息保障倍数
    i10:    // 营业收入增长率
    i11:    // 总资产增长率
    
    financialInput
}
```

后端发送：

```json
{
    o1:    // 流动性
    o2:    // 筹资
    o3:    // 清偿
    o4:    // 成长力
    o5:    // 财务风险综合指标
}
```



- 获取经营风险评估结果 **POST**
- http://localhost:8080/Model/GetBusinessRisk

前端发送：

```json
{
    i1:     // 销售净利率
    i2:     // 总资产报酬率
    i3:     // 净资产收益率
    i4:     // 存货周转率
    i5:     // 总资产周转率
    i6:     // 成本费用利润率
    i7:     // 诉讼数量(个)
    i8:     // 经营异常次数
    i9:     // 行政处罚次数
    i10:    // 动产质押次数
    i11:    // 自身风险
    i12:    // 周边风险
    i13:    // 股东数量（注册）
    i14:    // 第一大股东持股比例（注册）
}
```

后端发送：

```json
{
    o1:    // 经营力
    o2:    // 司法
    o3:    // 组织
    o4:    // 经营风险综合指标
}
```

