using SqlSugar;

namespace G_Forecast.Models
{
    [SugarTable("user")]
    public class User
    {
       

        [SugarColumn(IsPrimaryKey = true)]
        public int id { get; set; }  // 指定id_number为数据库表的主键,?表示可以为空
        public string? name { get; set; }

        public string? password { get; set; }

        public string? email { get; set; }


    }
}
