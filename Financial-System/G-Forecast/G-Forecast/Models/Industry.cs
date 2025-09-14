using SqlSugar;

namespace G_Forecast.Models
{
    [SugarTable("industry")]
    public class Industry  //行业表
    {
        [SugarColumn(IsPrimaryKey = true)]
        public int id { get; set; }

        public string code { get; set; }  //行业代码
        public string name { get; set; }  //行业名称
    }
}
