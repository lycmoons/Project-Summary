namespace G_Forecast.commom
{
    public class HttpStatusCodes
    {
        public const int OK = 200;
        public const int Created = 201;
        public const int NoContent = 204;

        public const int BadRequest = 400;
        public const int Unauthorized = 401;
        public const int Forbidden = 403;
        public const int NotFound = 404;
        public const int Conflict = 409;

        public const int InternalServerError = 500;
        public const int BadGateway = 502;
        public const int ServiceUnavailable = 503;

        // 继续扩展自定义业务码，比如：
        public const int CustomUserNotExist = 1001;
        public const int CustomDataConflict = 1002;
    }
}
