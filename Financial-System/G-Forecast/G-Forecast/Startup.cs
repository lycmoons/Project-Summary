using G_Forecast.Service;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.HttpsPolicy;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Microsoft.IdentityModel.Tokens;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Text;
using Microsoft.OpenApi.Models;

namespace G_Forecast
{
    public class Startup
    {
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        public void ConfigureServices(IServiceCollection services)
        {
            services.AddControllers();
            //跨域
            services.AddCors(options =>
            {
                options.AddPolicy("any",
                    builder =>
                    {
                        builder.AllowAnyOrigin()
                               .AllowAnyMethod()
                               .AllowAnyHeader();
                    });
            });

            services.AddControllers()
            .AddJsonOptions(options =>
            {
             options.JsonSerializerOptions.NumberHandling = System.Text.Json.Serialization.JsonNumberHandling.AllowNamedFloatingPointLiterals;
             });
            //添加swagger配置
            services.AddSwaggerGen(options =>
            {
                options.SwaggerDoc("v1",
                    new Microsoft.OpenApi.Models.OpenApiInfo
                    {
                        Title = "Api Swagger Doc",
                        Description = "try to use swagger build api doc",
                        Version = "v1"
                    });
                options.AddSecurityDefinition("JWTBearer", new OpenApiSecurityScheme()  //开启Authorize权限按钮
                {
                    Description = "这是方式一(直接在输入框中输入认证信息，不需要在开头添加Bearer) ",
                    Name = "Authorization",        //jwt默认的参数名称
                    In = ParameterLocation.Header,  //jwt默认存放Authorization信息的位置(请求头中)
                    Type = SecuritySchemeType.Http,
                    Scheme = "Bearer"
                });
                var scheme = new OpenApiSecurityScheme
                {
                    Reference = new OpenApiReference()
                    {
                        Id = "JWTBearer",  //这个名字与上面的一样
                        Type = ReferenceType.SecurityScheme
                    }
                };
                //注册全局认证（所有的接口都可以使用认证）
                options.AddSecurityRequirement(new OpenApiSecurityRequirement
                {
                    { scheme, Array.Empty<string>() }
                });

            });
            services.AddLogging(); // 添加日志服务
            services.AddMemoryCache(); //注册内存缓存服务
            services.AddScoped<EmailService>();//注册邮件相关服务

            //获取jwt配置项
            var jwtSettings = Configuration.GetSection("JwtSettings");

            // 添加jwt认证服务
            services.AddAuthentication(options =>
            {
                options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
                options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
            })
            .AddJwtBearer(options =>
            {
                options.TokenValidationParameters = new TokenValidationParameters
                {
                    ValidateIssuer = true,
                    ValidIssuer = jwtSettings["Issuer"],

                    ValidateAudience = true,
                    ValidAudience = jwtSettings["Audience"],

                    ValidateIssuerSigningKey = true,
                    IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(jwtSettings["SecretKey"])),

                    ValidateLifetime = true,  // 是否校验过期时间
                    ClockSkew = TimeSpan.Zero  // 过期时间容错设置，通常可设为0
                };
            });

            services.AddAuthorization();


        }

        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            //加入swagger中间件
            app.UseSwagger();
            app.UseSwaggerUI(options =>
            {
                options.SwaggerEndpoint("/swagger/v1/swagger.json", "MYBANK SWAGGER DOC");
            });
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseHttpsRedirection();

            app.UseRouting();

            app.UseCors("any");//允许跨域

            app.UseAuthentication();  // 注意顺序，必须在 UseAuthorization 之前

            app.UseAuthorization();

            app.UseMiddleware<JwtExceptionMiddleware>();
            //一定要写在 app.UseAuthentication() 和 app.UseAuthorization() 之后，这样能拦截 JWT 相关异常，并返回统一格式。

            // 特别处理 OPTIONS 预检请求，直接返回 200，防止进中间件
            app.Use(async (context, next) =>
            {
                if (context.Request.Method == HttpMethods.Options)
                {
                    context.Response.StatusCode = 200;
                    await context.Response.CompleteAsync();
                }
                else
                {
                    await next();
                }
            });

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapControllers();
            });
        }
    }
}