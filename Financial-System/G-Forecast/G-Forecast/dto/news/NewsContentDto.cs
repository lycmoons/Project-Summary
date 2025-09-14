namespace G_Forecast.dto
{
    public class NewsContentDto
    {
        

        public NewsContentDto(string id, string title, string time, string? content, string photo_url)
        {
            this.id = id;
            this.title = title;
            this.time = time;
            this.content = content;
            this.photo_url = photo_url;
        }

        public string id { get; set; }
        public string title { get; set; }  
      
        public string time { get; set; }  // 计算updatetime与现在时间的差距，返回说明字符串如："1 hour ago"、"20 minutes ago"...

        public string content { get; set; }

        public string? photo_url { get; set; }  
    }
}
