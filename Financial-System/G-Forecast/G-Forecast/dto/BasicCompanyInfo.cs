namespace G_Forecast.dto
{
    public class BasicCompanyInfo
    {
        public string uid { get; set; }  
        public string household_number { get; set; }  // 户号
        public string name { get; set; }  // 企业名称
        public string industry { get; set; }  // 企业领域
        public string scale { get; set; }  // 企业规模

        public string state { get; set; }  // 登记状态

        public int? score { get; set; }  // 企查分
    }
}
