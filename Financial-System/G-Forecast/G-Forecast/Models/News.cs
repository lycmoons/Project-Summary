using SqlSugar;

namespace G_Forecast.Models
{
    [SugarTable("news")]
    public class News
    {
        [SugarColumn(IsPrimaryKey = true)]
        public string id { get; set; }  
        public string? title { get; set; }

        public DateTime update_time { get; set; }

        public string? content { get; set; }
      
        public string? rel_block_name { get; set; }

        public string? img_url { get; set; }

        public string industry { get; set; }

    }
}
