using SqlSugar;

namespace G_Forecast.Models
{
    [SugarTable("companytag")]
    public class CompanyTag
    {
        [SugarColumn(IsPrimaryKey = true)]
        public int id { get; set; }

        public string company_id { get; set; }
        public string tag { get; set; }
    }
}
