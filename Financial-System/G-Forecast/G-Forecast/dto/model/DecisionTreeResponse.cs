namespace G_Forecast.dto.model
{
    public class DecisionTreeResponse
    {
        public List<double> debt_rate { get; set; }
        public List<double> interest { get; set; }
        public List<double> inventory_turn { get; set; }
        public List<double> flow_debt { get; set; }
        public List<double> cost { get; set; }
        public List<double> owner_equity { get; set; }
    }
}
