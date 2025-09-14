namespace G_Forecast.dto
{
    public class Chart // 用于绘制柱状图的数据
    {
        public List<DateTime>? time { get; set; }  // 对应这个行业的所有数据中date从小到大的排列

        public List<long>? volume { get; set; }  // 与上面date对应顺序的volume值（市值）

    }
}
