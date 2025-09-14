namespace G_Forecast.dto
{
    public class CompleteCompanyInfo
    {
        public string uid { get; set; }  // 指定id_number为数据库表的主键,?表示可以为空

        public string? household_number { get; set; }  // 户号

        public string? name { get; set; }  // 企业名称

        public string? industry { get; set; } //企业领域

        public string? scale { get; set; }  //企业规模

        public string? state { get; set; }  //登记状态

        public int? score { get; set; }  //企查分
        
        public string? credit { get; set; }   //统一社会信用代码

        public List<string>? tag { get; set; } //企业标签

        
        public string? date { get; set; } //成立日期

        public string? ddl { get; set; }  //经营期限

        public string? qualification { get; set; }  // 纳税人资质

        public string? registeredCapital { get; set; } // 注册资金

        public string? realCapital { get; set; }  // 实缴资金

        public string? type { get; set; }  // 企业类型

        public string? employeeScale { get; set; }  //人员规模

        public int? insuredEmployee { get; set; }  // 参保人数

        public string? nationalStandard { get; set; }  // 国标行业

        public string? businessScope { get; set; }  // 经营范围
    }
}
