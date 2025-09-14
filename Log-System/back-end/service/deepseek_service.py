from openai import OpenAI

class DeepseekServer:
    def __init__(self):
        DEEPSEEK_API_KEY = "deepseek-api-key"
        DEEPSEEK_API_URL = "https://api.deepseek.com"
        self.client = OpenAI(api_key=DEEPSEEK_API_KEY, base_url=DEEPSEEK_API_URL)

    '''
    功能：分析用户喜欢的新闻的种类和主题
    参数：用户对新闻点击的统计（lsit[UserStatistic]）
    返回值：返回分析出来的用户可能喜欢的category和topic（list[str]）
    '''
    def analyze_user_hobby(self, statistic_str: str) -> list[str]:
        pre_text = '现在我有一个用户的新闻点击曝光数据，内容是用户每一天对于不同的种类、不同的主题的新闻的点击量的统计。'
        mid_text = f'将数据整理成JSON格式如下：{statistic_str}。现在如果让你给用户推荐新闻，你会给用户推荐什么种类、什么主题的新闻。'
        end_text = '（请你直接给出一个推荐，不要你输出分析过程，给出答案即可，答案格式固定为：category topic。推荐的种类与推荐的主题以空格间隔，不需要引号，且推荐的种类和主题必须是我数据中有的。）'
        response = self.client.chat.completions.create(
            model="deepseek-chat",
            messages=[
                {"role": "system", "content": "You are a creative designer."},
                {"role": "user", "content": f'{pre_text}{mid_text}{end_text}'},
            ],
            stream=False
        )
        answer = response.choices[0].message.content
        ls = answer.split()
        print(answer)
        print(ls)
        return ls