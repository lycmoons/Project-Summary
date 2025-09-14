namespace G_Forecast.dto
{
    /// <summary>
    /// 决策树预测参数数据传输对象
    /// </summary>
    public class DecisionTreeRequest
    {


        public double cost { get; set; }

        public double debt_rate { get; set; }

        public double flow_debt { get; set; }

        /// <summary>
        /// EBITDA利息倍数（倍）
        /// </summary>
        public double interest { get; set; }

        /// <summary>
        /// 存货周转次数（次）
        /// </summary>
        public double inventory_turn { get; set; }

        /// <summary>
        /// 现金流动负债比率或EBITDA（%）
        /// </summary>
        

        /// <summary>
        /// 成本费用利润率（%）
        /// </summary>
        

        /// <summary>
        /// 所有者权益合计（万元）
        /// </summary>
        public double owner_equity { get; set; }
    }
}