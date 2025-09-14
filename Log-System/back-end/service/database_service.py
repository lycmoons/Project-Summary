from sqlalchemy import create_engine, text
from datetime import date, datetime
from entity.dto import Click, News, Query, ClickMax
from entity.vo import Condition
import json


class Mapper:
    def __init__(self):
        self.engine = create_engine("mysql+pymysql://username:password@localhost/BI_PROJECT")
    
    '''
    功能：获取当前时序
    参数：无
    返回值：当前时序日期（date）
    '''
    def getCurrentDate(self) -> date:
        with self.engine.connect() as conn:
            result = conn.execute(text('SELECT * FROM sequence'))
            for row in result:
                return row.cur_time
        return None
    
    '''
    功能：重置当前时序到2019/6/15
    参数：无
    返回值：是否成功（bool）
    '''
    def resetCurrentDate(self) -> bool:
        with self.engine.connect() as conn:
            result = conn.execute(text('UPDATE sequence SET cur_time = :time'), 
                                  {'time': date(2019, 6, 15)})
            if(result.rowcount == 0):
                return False
            conn.commit()
            return True
        return False
        
    '''
    功能：删除click表中2019/6/15之后的数据
    参数：无
    返回值：是否成功（bool）
    '''
    def resetClickTable(self) -> bool:
        with self.engine.connect() as conn:
            result = conn.execute(text('DELETE FROM click WHERE DATE(start_time) > :date'), 
                                  {'date': date(2019, 6, 15)})
            conn.commit()
        return True

    '''
    功能：时序日期加一
    参数：无
    返回值：是否成功（bool）
    '''
    def pushCurrentDate(self) -> bool:
        with self.engine.connect() as conn:
            result = conn.execute(text('UPDATE sequence SET cur_time = DATE_ADD(cur_time, INTERVAL 1 DAY)'))
            if(result.rowcount == 0):
                return False
            conn.commit()
            return True
        return False

    '''
    功能：向click表中添加一条数据
    参数：一条click封装数据（Click）
    返回值：是否成功（bool）
    '''
    def addClick(self, data: Click) -> bool:
        with self.engine.connect() as conn:
            result = conn.execute(text('INSERT INTO click VALUES (:user_id, :news_id, :start_time, :duration, :category, :topic, :title_len, :body_len)'),
                                  {'user_id': data.user_id,
                                   'news_id': data.news_id,
                                   'start_time': data.start_time,
                                   'duration': data.duration,
                                   'category': data.category,
                                   'topic': data.topic,
                                   'title_len': data.title_len,
                                   'body_len': data.body_len})
            if(result.rowcount == 0):
                return False
            conn.commit()
            return True
        return False
        
    '''
    功能：根据新闻id查询具体的一条新闻信息
    参数：新闻的id（str）
    返回值：一条新闻信息的封装类（News）
    '''
    def getNewsById(self, id: str) -> News:
        with self.engine.connect() as conn:
            result = conn.execute(text('SELECT * FROM news WHERE id = :id'),
                                  {'id': id})
            for row in result:
                return News(row.id, row.category, row.topic, row.title_len, row.body_len)
        return None
        
    

    '''
    功能：根据news_id获取其所有的点击时间
    参数：news_id（str）
    返回值：点击时间的列表（list[datetime]）
    '''
    def getClickByNewsId(self, news_id: str) -> list[datetime]:
        ls = []
        with self.engine.connect() as conn:
            start = datetime.now()
            result = conn.execute(text('SELECT start_time FROM click WHERE news_id = :news_id'),
                                  {'news_id': news_id})
            end = datetime.now()
            conn.execute(text('INSERT INTO query VALUES (:id, :content, :start, :end)'),
                               {'id': None,
                                'content': 'get life circle of a sigle news',
                                'start': start,
                                'end': end})
            conn.commit()
            for row in result:
                ls.append(row.start_time)
        return sorted(ls)

    '''
    功能：根据category获取其所有的点击时间
    参数：category（str）
    返回值：点击时间的列表（list[datetime]）
    '''
    def getClickByCategory(self, category: str) -> list[datetime]:
        ls = []
        with self.engine.connect() as conn:
            start = datetime.now()
            result = conn.execute(text('SELECT start_time FROM click WHERE category = :category'),
                                  {'category': category})
            end = datetime.now()
            conn.execute(text('INSERT INTO query VALUES (:id, :content, :start, :end)'),
                               {'id': None,
                                'content': 'get life circle of a sigle category of news',
                                'start': start,
                                'end': end})
            conn.commit()
            for row in result:
                ls.append(row.start_time)
        return sorted(ls)
    



    
    '''
    功能：根据user_id获取点击记录的category和点击时间
    参数：user_id（str）
    返回值：兴趣变化列表（list[2]）
    '''
    def getClickByUserId(self, user_id: str) -> list:
        ls = [[], []]
        with self.engine.connect() as conn:
            start = datetime.now()
            result = conn.execute(text('SELECT category, start_time FROM click WHERE user_id = :user_id'),
                                  {'user_id': user_id})
            end = datetime.now()
            conn.execute(text('INSERT INTO query VALUES (:id, :content, :start, :end)'),
                               {'id': None,
                                'content': 'get changes of user hobby',
                                'start': start,
                                'end': end})
            conn.commit()
            for row in result:
                ls[0].append(row.category)
                ls[1].append(row.start_time)
        return ls
    

    
    '''
    功能：根据复杂的筛选条件，获取新闻点击记录
    参数：筛选条件（Condition）
    返回值：得到的新闻点击记录的列表（list[Click]）
    '''
    def getClickByCondition(self, condition: Condition) -> list[Click]:
        ls = []
        sentence = 'SELECT * FROM click WHERE start_time >= :start AND start_time <= :end AND category IN :category AND title_len >= :min_title AND title_len <= :max_title AND body_len >= :min_body AND body_len <= :max_body AND user_id IN :user_id'
        with self.engine.connect() as conn:
            start = datetime.now()
            result = conn.execute(text(sentence),
                                  {'start': condition.time[0],
                                   'end': condition.time[1],
                                   'category': condition.category,
                                   'min_title': condition.title_len[0],
                                   'max_title': condition.title_len[1],
                                   'min_body': condition.body_len[0],
                                   'max_body': condition.body_len[1],
                                   'user_id': condition.user_id})
            end = datetime.now()
            conn.execute(text('INSERT INTO query VALUES (:id, :content, :start, :end)'),
                               {'id': None,
                                'content': 'get impression log by combined conditions',
                                'start': start,
                                'end': end})
            conn.commit()
            for row in result:
                ls.append(Click(row.user_id, row.news_id, row.start_time, row.duration, row.category, row.topic, row.title_len, row.body_len))
        return ls
    
    
    '''
    功能：获取不同的category和topic组合下的点击总数的最大值
    参数：无
    返回值：点击总数统计（ClickMax）
    '''
    def getClickSumGroupOnCategoryAndTopic(self) -> ClickMax:
        sentence = 'SELECT category, topic, COUNT(*) AS total FROM click GROUP BY category, topic ORDER BY total DESC LIMIT 1'
        with self.engine.connect() as conn:
            result = conn.execute(text(sentence))
            for row in result:
                return ClickMax(row.category, row.topic, row.total)
        return None
    
    '''
    功能：获取指定用户的点击历史的JSON格式统计数据
    参数：user_id（str）
    返回值：JSON格式的统计结果（str）
    '''
    def getClickSumGroupOnCategoryAndTopicByUserId(self, user_id: str) -> str:
        ls = []
        sentence = 'SELECT DATE(start_time) AS date, category, topic, COUNT(*) AS cnt FROM click WHERE user_id = :user_id GROUP BY DATE(start_time), category, topic'
        with self.engine.connect() as conn:
            result = conn.execute(text(sentence), {'user_id': user_id})
            for row in result:
                ls.append({'date': row.date.isoformat(), 'category': row.category, 'topic': row.topic, 'cnt': row.cnt})
        return json.dumps(ls)
    
    '''
    功能：根据category和topic查询新闻记录
    参数：category（str），topic（str）
    返回值：新闻记录列表（list[News]）
    '''
    def getNewsByCategoryTopic(self, category: str, topic: str) -> list[News]:
        ls = []
        sentence = 'SELECT * FROM news WHERE category = :category AND topic = :topic LIMIT 100'
        with self.engine.connect() as conn:
            result = conn.execute(text(sentence),
                                  {
                                      'category': category,
                                      'topic': topic
                                  })
            for row in result:
                ls.append(News(row.id, row.category, row.topic, row.title_len, row.body_len))
        return ls

    '''
    功能：获取所有的查询日志
    参数：无
    返回值：查询日志的列表（list[Query]）
    '''
    def getAllQuery(self) -> list[Query]:
        ls = []
        sentence = 'SELECT * FROM query'
        with self.engine.connect() as conn:
            result = conn.execute(text(sentence))
            for row in result:
                ls.append(Query(row.id, row.content, row.start, row.end))
        return ls