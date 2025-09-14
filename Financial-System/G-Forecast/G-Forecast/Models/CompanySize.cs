using SqlSugar;

namespace G_Forecast.Models
{
    [SugarTable("companysize")]
    public class CompanySize
    {
        [SugarColumn(IsPrimaryKey = true)]
        public int id { get; set; }
        public string size { get; set; }
    }
}
