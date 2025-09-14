using Microsoft.AspNetCore.Cors;
using Microsoft.AspNetCore.Mvc;
using SqlSugar;
using System.Numerics;
using G_Forecast.Models;
using System.Text.Json;
using System.Text.Json.Serialization;
using Microsoft.Extensions.Logging;
using G_Forecast.dto;
using Microsoft.Extensions.Caching.Memory;
using G_Forecast.Service;
using Microsoft.IdentityModel.Tokens;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using System.Text;
using Microsoft.AspNetCore.Authorization;
using G_Forecast.commom;
using Azure;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace G_Forecast.Controllers
{
    [Route("/[controller]/[action]")]
    [ApiController]
    [EnableCors("any")]
    public class NewsController : ControllerBase
    {
        private readonly SqlSugarClient _db;
        private readonly ILogger<NewsController> _logger; // 声明日志记录器

        public NewsController(ILogger<NewsController> logger)
        {

            _logger = logger; // 初始化日志记录器
            //获取数据库连接
            dbORM dborm = new dbORM();
            _db = dborm.getInstance();
        }

        // 获取所有新闻的基本信息
        //[Authorize]
        [HttpGet]
        public async Task<IActionResult> GetAllNews()
        {
            try
            {
                List<NewsQueryDto> queryList = await _db.Queryable<News>()
               .OrderBy(n => n.id)  //按uid升序
               .Select(n => new NewsQueryDto
               {
                   id = n.id,
                   photo_url=n.img_url,
                   title = n.title,                   
                   type=n.rel_block_name,
                   updatetime = n.update_time // 查出来先放着
               })
               .ToListAsync();

                List<NewsDto> news = queryList.Select(item => new NewsDto
                {
                    id = item.id,
                    photo_url = item.photo_url,
                    title = item.title,
                    type = item.type,
                    time = GetTimeAgo(item.updatetime)
                }).ToList();

                AllNews allNews=new AllNews();
                allNews.news=news; 
                _logger.LogInformation("查询所有新闻的基本信息");
                ApiResponse response = new ApiResponse(HttpStatusCodes.OK, allNews, "查询所有新闻的基本信息成功");
                return Ok(response);
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation(ex, "查询一个新闻的具体内容失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "查询所有新闻的基本信息失败");
                return BadRequest(response); //false表示失败
            }
        }
        //获取某一个新闻的具体内容
        [HttpPost]
        public async Task<IActionResult> GetNews([FromBody] GetNewsRequest getNewsRequest)
        {
            try
            {
                string id = getNewsRequest.id;
                NewsQueryDto query = await _db.Queryable<News>()
               .Where(n => n.id == id)
               .OrderBy(n => n.id)  //按uid升序
               .Select(n => new NewsQueryDto
               {
                   id = n.id,
                   photo_url = n.img_url,
                   content=n.content,
                   title = n.title,
                   type = n.rel_block_name,
                   updatetime = n.update_time // 查出来先放着
               })
               .FirstAsync();
           
                NewsContentDto news = new NewsContentDto
                (
                    query.id,
                    query.title,
                    GetTimeAgo(query.updatetime),
                    query.content,
                    query.photo_url
                   
                );

                _logger.LogInformation("查询一个新闻的具体内容成功");
                ApiResponse response = new ApiResponse(HttpStatusCodes.OK, news, "查询一个新闻的具体内容成功");
                return Ok(response);
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation(ex, "查询一个新闻的具体内容失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "查询一个新闻的具体内容失败");
                return BadRequest(response); //false表示失败
            }
        }

        // 计算updatetime与现在时间的差距，返回说明字符串如："1 hour ago"、"20 minutes ago"...
        private string GetTimeAgo(DateTime time)
        {
            TimeSpan ts = DateTime.Now - time;
            if (ts.TotalDays >= 1)
                return $"{(int)ts.TotalDays} day(s) ago";
            else if (ts.TotalHours >= 1)
                return $"{(int)ts.TotalHours} hour(s) ago";
            else if (ts.TotalMinutes >= 1)
                return $"{(int)ts.TotalMinutes} minute(s) ago";
            else
                return "just now";
        }



    }
}
