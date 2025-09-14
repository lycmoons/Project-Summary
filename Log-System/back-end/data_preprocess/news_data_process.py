import pandas as pd
from sqlalchemy import create_engine

# 读取
news = pd.read_csv('/Users/aidon/Desktop/BI-Project/data/PENS/news.tsv', sep='\t')

# 数据预处理
news['News body'] = news['News body'].fillna('')
news['title_len'] = news['Headline'].str.len()
news['body_len'] = news['News body'].str.len()
news = news.rename(columns={"News ID": "id", 'Category': 'category', 'Topic': 'topic'})
news = news[['id', 'category', 'topic', 'title_len', 'body_len']]

# 导入数据库
user = 'root'
password = '123456'
host = 'localhost'
database = 'BI_PROJECT'
engine = create_engine(f'mysql+pymysql://{user}:{password}@{host}/{database}')
news.to_sql(name='news', con=engine, if_exists="append", index=False)