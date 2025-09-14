namespace G_Forecast.dto
{
    public class ListedCompanyGraphDto
    {
        public List<string> yearList { get; set; } // 从小到大的年份（字符串）

        public List<double> profits { get; set; }  // 对应年份该公司的归母净利润

        public List<int> rate { get; set; }  // 对应年份该公司的同比
    }
}
