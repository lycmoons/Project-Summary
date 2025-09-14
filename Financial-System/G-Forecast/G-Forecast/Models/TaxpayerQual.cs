using SqlSugar;

namespace G_Forecast.Models
{
    [SugarTable("taxpayerqual")]
    public class TaxpayerQual  //纳税人资质表
    {
        [SugarColumn(IsPrimaryKey = true)]
        public int id { get; set; }
        public string name { get; set; } //资质名称（如一般纳税人、小规模纳税人）

    }
}
