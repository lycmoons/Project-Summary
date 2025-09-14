## 系统设计架构图





## 接口设计

- 获取当前时序 **GET**
- **http://localhost:8000/cur/time**

后端发送：

```json
{
    cur_date: date
}
```



- 时序重置操作 **GET**
- **http://localhost:8000/reset/time**

后端发送：

```json
{
    status: bool  // true表示成功，false表示失败
}
```



- 时序加一天操作 **GET**
- **http://localhost:8000/add/time**

后端发送：

```json
{
    status: bool  // true表示成功，false表示失败
}
```



- 获取某个新闻的生命周期 **POST**
- **http://localhost:8000/lifeCircle/byNews**

前端发送：

```json
{
    id: str
}
```

后端发送：

```json
{
    lifeCircle: list[datetime]
}
```



- 获取某个种类新闻的生命周期 **POST**
- **http://localhost:8000/lifeCircle/byCategory**

前端发送：

```json
{
    category: str
}
```

后端发送：

```json
{
    lifeCircle: list[datetime]
}
```



- 获取某个用户兴趣变化 **POST**
- **http://localhost:8000/hobby**

前端发送：

```json
{
    id: str
}
```

后端发送：

```json
{
    date: [date1, date2, ...],    // datetime类型
    data: [
           // date1的每个种类新闻的点击统计
           {
               category: [category1, category2,...],  // str类型
    		   sum: [sum1, sum2,...]     // int类型
           },
                     
           // date2的每个种类新闻的点击统计
           {
               category: [category1, category2,...],
    		   sum: [sum1, sum2,...]
           },
           ...
          ]
}
```



- 根据组合条件获取新闻曝光日志 **POST**
- **http://localhost:8000/condition/check**

前端发送：

```json
{
    time: list[datetime],  // 表示查询 list[0] ～ list[1] 的时间范围的记录
    category: list[str],   // 表示查询列表中存在的种类
    title_len: list[int],  // 表示查询标题长度在 list[0] ～ list[1] 的记录
    body_len: list[int],   // 表示查询内容长度在 list[0] ～ list[1] 的记录
    user_id: list[str],    // 表示查询列表中存在的用户的记录
}
```

后端发送：

```json
{
    records: [
        // 一条记录的信息
        {
            user_id: str,
            news_id: str,
            start_time: datetime,
            duration: int,
            category: str,
            topic: str,
            title_len: int,
            body_len: int
        },
        // ...更多记录
    ]
}
```



- 爆款新闻分析 **GET**
- **http://localhost:8000/popularity**

后端发送：

```json
{
    category: str,   // 爆款新闻的种类
    topic: str,      // 爆款新闻的主题
    news: [{         // 查出来的爆款新闻的条目
        	id: str
    		category: str
    		topic: str
    		title_len: int
    		body_len: int
    },{}, ...]
}
```



- 用户新闻推荐 **POST**
- **http://localhost:8000/popularity/byUser**

前端发送：

```json
{
    id: str
}
```

后端发送：

```json
{
    category: str,   // 用户喜欢新闻的种类
    topic: str,      // 用户喜欢新闻的主题
    news: [{         // 查出来的用户喜欢新闻的条目
        	id: str
    		category: str
    		topic: str
    		title_len: int
    		body_len: int
    },{}, ...]
}
```



- 获取所有的查询日志 **GET**
- **http://localhost:8000/log**

后端发送：

```json
{
    logs: [{
        id: int,
        content: str,
        start: datetime,
        end: datetime
    },{...}]
}
```



## 数据库表设计

- 曝光日志表

```sql
create table click
(
    user_id    varchar(255) not null,
    news_id    varchar(255) not null,
    start_time datetime     not null,
    duration   int          not null,
    category   varchar(255) not null,
    topic      varchar(255) not null,
    title_len  int          not null,
    body_len   int          not null,
    primary key (user_id, news_id, start_time)
);
```



- 新闻表

```sql
create table news
(
    id        varchar(255) not null
        primary key,
    category  varchar(255) null,
    topic     varchar(255) null,
    title_len int          null,
    body_len  int          null
);
```



- 查询日志表

```sql
create table query
(
    id      int auto_increment
        primary key,
    content varchar(255) not null,
    start   datetime     not null,
    end     datetime     not null
);
```



## 索引设计

```sql
CREATE INDEX idx_1 ON click(news_id);
CREATE INDEX idx_2 ON click(category, topic);
CREATE INDEX idx_3 ON click(user_id);
CREATE INDEX idx_4 ON click(start_time, title_len, body_len);
CREATE INDEX idx_5 ON news(category, topic);
```

