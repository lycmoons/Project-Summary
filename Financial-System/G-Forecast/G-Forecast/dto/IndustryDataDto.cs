namespace G_Forecast.dto
{
    public class IndustryDataDto
    {
        public DateTime? date { get; set; } // 日期

        public string? code { get; set; } // 行业代码

        public double? open { get; set; }  // 开盘价

        public double? close { get; set; }  // 收盘价
        public long? turnover { get; set; } //成交量

        public double? totalshare { get; set; }  // 总股数
    }
}
