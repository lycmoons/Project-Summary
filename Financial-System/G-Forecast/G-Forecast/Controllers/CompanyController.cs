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
using Newtonsoft.Json;
using G_Forecast.dto.company;

namespace G_Forecast.Controllers
{
    [Route("/[controller]/[action]")]
    [ApiController]
    [EnableCors("any")]
    public class CompanyController : ControllerBase
    {
        private readonly SqlSugarClient _db;
        private readonly ILogger<CompanyController> _logger; // 声明日志记录器
      
        public CompanyController(ILogger<CompanyController> logger)
        {

            _logger = logger; // 初始化日志记录器
            //获取数据库连接
            dbORM dborm = new dbORM();
            _db = dborm.getInstance();
        }

        // 获取所有公司的基本信息
        //[Authorize]
        [HttpGet]
        public async Task<IActionResult> GetAllCompaniesInfo()
        {
            try
            {
                List<BasicCompanyInfo> basicCompanyInfos = await _db.Queryable<Company>()
               .LeftJoin<CompanySize>((p, e) => p.size_id == e.id)             
               .OrderBy((p, e) => p.uid)  //按uid升序
               .Select((p, e) => new BasicCompanyInfo
               {
                   uid = p.uid,
                   household_number = p.household_number,
                   name = p.name,
                   industry = p.industry,
                   scale = e.size,
                   state = p.status,
                   score = p.score
               })
               .ToListAsync();

                AllCompanies allCompanies= new AllCompanies();
                allCompanies.companies = basicCompanyInfos;
                _logger.LogInformation("查询所有公司的基本信息成功"); 

                ApiResponse response = new ApiResponse(HttpStatusCodes.OK, allCompanies, "查询所有公司的基本信息成功");
                return Ok(response); 
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation("查询所有公司的基本信息失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "查询所有公司的基本信息失败");
                return BadRequest(response); //false表示失败
            }
        }

        // 查询指定公司的所有信息
        //[Authorize]
        [HttpPost]
        public async Task<IActionResult> GetCompanyInfo([FromBody] GetCompanyInfoRequest getCompanyInfoRequest)
        {
            string uid = getCompanyInfoRequest.uid;
            try
            {
                

                List<string> companyTags = await _db.Queryable<CompanyTag>()
                .Where(t => t.company_id == uid)
                .Select(t => t.tag)
                .ToListAsync();
                _logger.LogInformation(companyTags[0]);

                CompleteCompanyInfo completeCompanyInfo = await _db.Queryable<Company>()
               .Where(c => c.uid == uid)
               .LeftJoin<CompanySize>((c, s) => c.size_id == s.id)
               .LeftJoin<TaxpayerQual>((c, s, t1) => c.taxpayer_qual_id == t1.id)
               .LeftJoin<CompanyType>((c, s, t1, t2) => c.type_id == t2.id)
               .Select((c, s, t1, t2) => new CompleteCompanyInfo
               {
                   uid = c.uid,
                   household_number = c.household_number,
                   name = c.name,
                   industry = c.industry,
                   scale = s.size,
                   state = c.status,
                   score = c.score,
                   credit = c.credit_code,
                   tag = companyTags,
                   date = c.establish_date,
                   ddl = c.operation_period,
                   qualification = t1.name,
                   registeredCapital = c.registered_capital,
                   realCapital = c.paid_capital,
                   type= t2.name,
                   employeeScale= c.staff_size,
                   insuredEmployee = c.insured_count,
                   nationalStandard =c.nationalstandard,
                   businessScope = c.business_scope
               }).FirstAsync();

                // 最后把标签赋值进去,companyTags 是 C# 内存变量，SqlSugar 的 SQL 翻译器看不到它
                completeCompanyInfo.tag = companyTags;

                _logger.LogInformation("查询指定公司的所有信息成功：uid：{uid}",uid);
                ApiResponse response = new ApiResponse(HttpStatusCodes.OK, completeCompanyInfo, "查询指定公司的所有信息成功");
                return Ok(response);
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation(ex,"查询指定公司的所有信息失败：uid：{uid}", uid);
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "查询指定公司的所有信息失败");
                return BadRequest(response); //false表示失败
            }
        }

        //获取所有行业的上市公司
        [HttpGet]
        public async Task<IActionResult> GetListedCompanies()
        {
            try
            {
                List<ListedCompanyInfo> listedCompanies = await _db.Queryable<Company>()
               .Where(n=>n.stock!="无")
               .OrderBy(n => n.uid)  //按uid升序
               .Select(n => new ListedCompanyInfo
               {
                   uid = n.uid,
                   name = n.name,
                   industry = n.industry,                  
               })
               .ToListAsync();

                ListedCompaniesResult listedCompaniesResult= new ListedCompaniesResult();
                listedCompaniesResult.companies= listedCompanies;
                _logger.LogInformation("查询行业的上市公司成功");
                ApiResponse response = new ApiResponse(HttpStatusCodes.OK, listedCompaniesResult, "查询行业的上市公司成功");
                return Ok(response);
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation(ex, "查询行业的上市公司失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "查询行业的上市公司失败");
                return BadRequest(response); //false表示失败
            }
        }

        //获取某个上市公司股票市值的变化
        [HttpPost]
        public async Task<IActionResult> GetStockChange([FromBody] GetCompanyInfoRequest getCompanyInfoRequest)
        {
            string uid = getCompanyInfoRequest.uid;

            try
            {
                //获取该公司的股票代码
                string stock = await _db.Queryable<Company>()
                .Where(c => c.uid == uid)
                .Select(c => c.stock)
                .FirstAsync();

                stock = stock + ".SH";
                using (HttpClient client = new HttpClient())
                {
                    // 假设你的 Flask 服务运行在 localhost:5000
                    string flaskApiUrl = $"http://113.44.84.116:5000/stock_info?code={stock}";

                    HttpResponseMessage responseMessage = await client.GetAsync(flaskApiUrl);
                    if (responseMessage.IsSuccessStatusCode)
                    {
                        string resultJson = await responseMessage.Content.ReadAsStringAsync();

                        // 3. 反序列化成对象（或者直接返回）
                        var companyGraphDto = JsonConvert.DeserializeObject<CompanyGraphDto>(resultJson);

                        
                        CompanyStockChanges companyData = new CompanyStockChanges();

                        companyData.dateList = new List<string>(companyGraphDto.yearList);

                        companyData.value = new List<double>(companyGraphDto.money);
                        _logger.LogInformation("获取上市公司股票市值的变化成功");

                        ApiResponse response = new ApiResponse(HttpStatusCodes.OK, companyData, "获取上市公司股票市值的变化成功");
                        return Ok(response);
                    }
                    else
                    {
                        _logger.LogError("调用 Python 接口失败: " + responseMessage.StatusCode);
                        ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取上市公司股票市值的变化失败，调用Python服务异常");
                        return BadRequest(response);
                    }
                }
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation(ex, "获取上市公司股票市值的变化失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取上市公司股票市值的变化失败");
                return BadRequest(response); //false表示失败
            }
        }


        //获取某个公司相关的新闻信息
        [HttpPost]
        public async Task<IActionResult> GetCompanyNews([FromBody] GetCompanyInfoRequest getCompanyInfoRequest)
        {
            string uid = getCompanyInfoRequest.uid;
            try
            {
                //获取该公司的领域
                string industry = await _db.Queryable<Company>()
                .Where(c => c.uid == uid)
                .Select(c => c.industry)
                .FirstAsync();

                _logger.LogInformation("获取某个公司相关的新闻信息：成功获取公司{industry}的领域", industry);

                var newsList = await _db.Queryable<News>()
                 .Where(it => it.industry.Contains(industry))
                 .ToListAsync();

                

                List<NewsDto> news = newsList.Select(item => new NewsDto
                {
                    id = item.id,
                    photo_url = item.img_url,
                    title = item.title,
                    time = GetTimeAgo(item.update_time),
                    type = item.rel_block_name
                }).ToList();

                CompanyNews companyNews = new CompanyNews();
                companyNews.news= news;

                ApiResponse response = new ApiResponse(HttpStatusCodes.OK, companyNews, "获取某个公司相关的新闻信息成功");
                 return Ok(response);
                    
                
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation(ex, "获取某个公司相关的新闻信息失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取某个公司相关的新闻信息失败");
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
