import pandas as pd
from datetime import date


'''
功能：将对应日期的新闻曝光日志写入flume监听的文件中
参数：需要写入的日期（date）
返回值：无
'''
def write_log(dt: date):
    path = f'/Users/aidon/Desktop/BI-Project/impression_log/{dt.year}-{dt.month}-{dt.day}.csv'
    data = pd.read_csv(path)
    with open('/Users/aidon/Desktop/BI-Project/flume_service/flume-log.txt', 'a') as f_log:
        for row in data.itertuples():
            line = f'{row.user_id},{row.news_id},{row.start_time},{row.duration}\n'
            f_log.write(line)
            f_log.flush()