using SqlSugar;

namespace G_Forecast.Models
{
    [SugarTable("company")]
    public class Company
    {
        [SugarColumn(IsPrimaryKey = true)]
        public string uid { get; set; }  // 主键，唯一标识企业
        public string? household_number { get; set; }  //户号

        public string? name { get; set; }  //企业名称

        public string? credit_code { get; set; }  //统一社会信用代码

        

        public int? size_id { get; set; }  //企业规模

        public int? score { get; set; }  //企查分
        public string? status { get; set; }  //登记状态

        public string? establish_date { get; set; }  //成立日期

        public string? operation_period { get; set; }  //营业期限

        public int? taxpayer_qual_id { get; set; }  //纳税人资质

        public string? registered_capital { get; set; } //注册资本（含单位）

        public string? paid_capital { get; set; } //实缴资本（含单位）

        public int? type_id { get; set; } //企业类型

        public string? staff_size { get; set; } //人员规模

        public int? insured_count { get; set; } //参保人数

        public string? nationalstandard { get; set; }  //国标行业

        public string? business_scope { get; set; }  //经营范围

        public string industry { get; set; }  //企业领域

        public string stock { get; set; }  //股票代码

    }
}
