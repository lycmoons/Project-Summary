namespace G_Forecast.dto
{
    public class NewsDto
    {
        public string id { get; set; }
        public string photo_url { get; set; }  // img_url

        public string title { get; set; } // title

        public string time { get; set; }  // 计算updatetime与现在时间的差距，返回说明字符串如："1 hour ago"、"20 minutes ago"...

        public string type { get; set; }  // rel_block_name

    }
}
