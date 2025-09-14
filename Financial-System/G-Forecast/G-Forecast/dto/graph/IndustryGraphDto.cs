namespace G_Forecast.dto
{
    public class IndustryGraphDto
    {
        public Chart? chart{ get; set; }  // 用于绘制柱状图的数据

        public List<IndustryDataDto>? table { get; set; }// 用于绘制表格的数据
    }
}
