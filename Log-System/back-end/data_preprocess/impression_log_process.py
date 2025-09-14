import pandas as pd
from datetime import datetime, timedelta

train = pd.read_csv('/Users/aidon/Desktop/BI-Project/data/PENS/train.tsv', sep='\t')
data = train[['UserID','ClicknewsID','exposure_time','dwelltime','pos','start','dwelltime_pos']]

def transfer_to_datetime_list(date_str):
    datetime_template = '%m/%d/%Y %H:%M:%S'
    ls = date_str.split("#TAB#")
    datetime_list = []
    for s in ls:
        datetime_list.append(datetime.strptime(s[:-3], datetime_template))
    return datetime_list

def transfer_to_int_list(int_str):
    return [int(i) for i in int_str.split()]

def transfer_to_newsid_list(newsid_str):
    return newsid_str.split()

def calculate_datetime(start_str, dwelltime):
    ls = transfer_to_datetime_list(start_str)
    for i in range(len(dwelltime) - 1):
        ls.append(ls[-1] + timedelta(seconds=dwelltime[i]))
    return ls

data['ClicknewsID'] = data['ClicknewsID'].apply(transfer_to_newsid_list)
data['exposure_time'] = data['exposure_time'].apply(transfer_to_datetime_list)
data['dwelltime'] = data['dwelltime'].apply(transfer_to_int_list)
data['pos'] = data['pos'].apply(transfer_to_newsid_list)
data['dwelltime_pos'] = data['dwelltime_pos'].apply(transfer_to_int_list)
data['start'] = data.apply(lambda row: calculate_datetime(row['start'], row['dwelltime_pos']), axis=1)

data_sorted = data.sort_values(by='start', key=lambda x: x.str[0], ascending=False)
data_sorted.reset_index(drop=True, inplace=True)
data_unique = data_sorted.drop_duplicates(subset='UserID', keep='first')
data_unique.reset_index(drop=True, inplace=True)

d = []
for i in range(data_unique.shape[0]):
    for j in range(len(data_unique.loc[i, 'ClicknewsID'])):
        d.append((data_unique.loc[i, 'UserID'], data_unique.loc[i, 'ClicknewsID'][j],
                  data_unique.loc[i, 'exposure_time'][j], data_unique.loc[i, 'dwelltime'][j]))
    for j in range(len(data_unique.loc[i, 'pos'])):
        d.append((data_unique.loc[i, 'UserID'], data_unique.loc[i, 'pos'][j],
                  data_unique.loc[i, 'start'][j], data_unique.loc[i, 'dwelltime_pos'][j]))
        
impression_log_columns = ['user_id', 'news_id', 'start_time', 'duration']
impression_log = pd.DataFrame(d, columns=impression_log_columns)

impression_log['date'] = impression_log['start_time'].dt.date
for date, group in impression_log.groupby('date'):
    filename = f"{date}.csv"
    group.drop('date', axis=1).to_csv(filename, index=False)