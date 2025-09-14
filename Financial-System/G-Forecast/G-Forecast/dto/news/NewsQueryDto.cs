namespace G_Forecast.dto
{
    public class NewsQueryDto
    {
        public string id { get; set; }
        public string photo_url { get; set; }
        public string title { get; set; }
        public string type { get; set; }
        public DateTime updatetime { get; set; }

        public string? content { get; set; }
    }
}
