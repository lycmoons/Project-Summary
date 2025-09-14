from fastapi import FastAPI
from entity.vo import NewsId, NewsCategory, UserId, Condition
from service.database_service import Mapper
from collections import defaultdict
from service.log_simulate import write_log
from fastapi.middleware.cors import CORSMiddleware
from service.deepseek_service import DeepseekServer

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://127.0.0.1:5500"],  # 前端地址，或使用 ["*"] 允许所有
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

mapper = Mapper()
deepseek = DeepseekServer()

# 获取当前的时序
@app.get('/cur/time')
async def getCurrentTime():
    d = mapper.getCurrentDate()
    return {
        'cur_date': d
    }

# 时序重置操作
@app.get('/reset/time')
async def getCurrentTime():
    a = mapper.resetCurrentDate()
    b = mapper.resetClickTable()
    return {
        'status': a and b
    }

# 时序加一天操作
@app.get('/add/time')
async def getCurrentTime():
    a = mapper.pushCurrentDate()
    if a == False:
        return {'status': False}    
    d = mapper.getCurrentDate()
    write_log(d)
    return {
        'status': True
    }

# 单个新闻生命周期查询
@app.post('/lifeCircle/byNews')
async def getLifeCircleByNews(req: NewsId):
    ls = mapper.getClickByNewsId(req.id)
    return {
        'lifeCircle': ls
    }

# 某个种类新闻生命周期查询
@app.post('/lifeCircle/byCategory')
async def getLifeCircleByCategory(req: NewsCategory):
    ls = mapper.getClickByCategory(req.category)
    return {
        'lifeCircle': ls
    }

# 某个用户兴趣爱好变化
@app.post('/hobby')
async def getHobbyChangeByUserId(req: UserId):
    ls = mapper.getClickByUserId(req.id)
    temp = defaultdict(lambda: defaultdict(int))
    for cat, dt in zip(ls[0], ls[1]):
        day = dt.date()
        temp[day][cat] += 1
    result = {
        'date': [],
        'data': []
    }
    for day in sorted(temp.keys()):
        result['date'].append(day)
        day_data = temp[day]
        result['data'].append({
            'category': list(day_data.keys()),
            'sum': list(day_data.values())
        })
    return result


# 多组合筛选条件查询
@app.post('/condition/check')
async def getClickByCondition(req: Condition):
    ls = mapper.getClickByCondition(req)
    res = []
    for c in ls:
        res.append({
            'user_id': c.user_id,
            'news_id': c.news_id,
            'start_time': c.start_time,
            'duration': c.duration,
            'category': c.category,
            'topic': c.topic,
            'title_len': c.title_len,
            'body_len': c.body_len
        })
    return {
        'records': res
    }


# 爆款新闻分析
@app.get('/popularity')
async def getPopularNews():
    clickMax = mapper.getClickSumGroupOnCategoryAndTopic()
    ls = mapper.getNewsByCategoryTopic(clickMax.category, clickMax.topic)
    news = []
    for n in ls:
        news.append({
            'id': n.id,
            'category': n.category,
            'topic': n.topic,
            'title_len': n.title_len,
            'body_len': n.body_len
        })

    return {
        'category': clickMax.category,
        'topic': clickMax.topic,
        'news': news
    }

# 用户新闻推荐
@app.post('/popularity/byUser')
async def getPopularNewsByUserId(req: UserId):
    statistic_str = mapper.getClickSumGroupOnCategoryAndTopicByUserId(req.id)
    res = deepseek.analyze_user_hobby(statistic_str)
    ls = mapper.getNewsByCategoryTopic(res[0], res[1])
    news = []
    for n in ls:
        news.append({
            'id': n.id,
            'category': n.category,
            'topic': n.topic,
            'title_len': n.title_len,
            'body_len': n.body_len
        })

    return {
        'category': res[0],
        'topic': res[1],
        'news': news
    }

# 获取查询日志
@app.get('/log')
async def getQueryLog():
    ls = mapper.getAllQuery()
    logs = []
    for l in ls:
        logs.append({
            'id': l.id,
            'content': l.content,
            'start': l.start,
            'end': l.end
        })
    return {
        'logs': logs
    }