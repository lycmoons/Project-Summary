namespace G_Forecast.dto
{
    // 邮箱验证码请求类
    public class VerifyCodeRequest
    {
        public string Email { get; set; }
        public string Code { get; set; }
    }
}
