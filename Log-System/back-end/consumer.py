from kafka import KafkaConsumer
from service.database_service import Mapper
from entity.dto import Click
from datetime import datetime


mapper = Mapper()



consumer = KafkaConsumer(
    'click_logs',
    bootstrap_servers=['localhost:9092'],
    auto_offset_reset='latest',
    enable_auto_commit=True,
    group_id='mysql-writer',
    value_deserializer=lambda x: x.decode('utf-8')
)



for message in consumer:
    user_id, news_id, start_time, duration = message.value.strip().split(',')
    start_time = datetime.strptime(start_time, '%Y-%m-%d %H:%M:%S')
    duration = int(duration)
    news = mapper.getNewsById(news_id)
    mapper.addClick(Click(user_id,news_id,start_time,duration,news.category,news.topic,news.title_len,news.body_len))
    print(f'[INFO]: insert ({user_id}, {news_id}) successfully...')