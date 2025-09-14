using SqlSugar;

namespace G_Forecast.Models
{
    [SugarTable("companytype")]
    public class CompanyType
    {
        [SugarColumn(IsPrimaryKey = true)]
        public int id { get; set; }
        public string name { get; set; }   //类型名称（如其他有限责任公司、外国法人独资）

    }
}
