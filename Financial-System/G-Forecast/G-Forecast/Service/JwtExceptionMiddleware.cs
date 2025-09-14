using Microsoft.IdentityModel.Tokens;
using System.Text.Json;
using G_Forecast.dto;
using G_Forecast.commom;

namespace G_Forecast.Service
{
    public class JwtExceptionMiddleware   //拦截 JWT 校验失败统一返回
    {
        private readonly RequestDelegate _next;
        private readonly ILogger<JwtExceptionMiddleware> _logger;

        public JwtExceptionMiddleware(RequestDelegate next, ILogger<JwtExceptionMiddleware> logger)
        {
            _next = next;
            _logger = logger;
        }

        public async Task Invoke(HttpContext context)
        {
            try
            {
                await _next(context);
            }
            catch (SecurityTokenException ex)
            {
                _logger.LogError(ex, "JWT 校验失败");

                context.Response.StatusCode = 401;
                context.Response.ContentType = "application/json";

                var response = new ApiResponse(HttpStatusCodes.Unauthorized,null, "Token无效或已过期");
                await context.Response.WriteAsync(JsonSerializer.Serialize(response));
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "系统异常");

                context.Response.StatusCode = 500;
                context.Response.ContentType = "application/json";

                var response = new ApiResponse(HttpStatusCodes.InternalServerError, null, "系统异常");
                await context.Response.WriteAsync(JsonSerializer.Serialize(response));
            }
        }
    }
}
