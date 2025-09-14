namespace G_Forecast.dto
{

    public class ApiResponse
    {
        public int Code { get; set; }
        public object? Data { get; set; }
        public string Message { get; set; }

        public ApiResponse(int code, object data, string message) {
            Code = code;
            Data= data;
            Message = message;
        }
    }

}
