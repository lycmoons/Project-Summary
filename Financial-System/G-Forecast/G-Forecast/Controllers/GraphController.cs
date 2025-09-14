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
using System.Runtime.InteropServices;
using Newtonsoft.Json;

namespace G_Forecast.Controllers
{
    [Route("/[controller]/[action]")]
    [ApiController]
    [EnableCors("any")]
    public class GraphController : ControllerBase
    {
        private readonly SqlSugarClient _db;
        private readonly ILogger<GraphController> _logger; // 声明日志记录器
        private readonly IMemoryCache _cache;
        private readonly EmailService _emailService;
        private readonly IConfiguration _configuration;

        public GraphController(ILogger<GraphController> logger, IMemoryCache cache, EmailService emailService, IConfiguration configuration)
        {
            _cache = cache;
            _emailService = emailService;

            _configuration = configuration;

            _logger = logger; // 初始化日志记录器
            //获取数据库连接
            dbORM dborm = new dbORM();
            _db = dborm.getInstance();
        }

        //获取某个行业的图表所需数据
        [HttpPost]
        public async Task<IActionResult> GetIndustryGraphData([FromBody] GetIndustryGraphDataRequest getIndustryGraphDataRequest)
        {
            string industry = getIndustryGraphDataRequest.industry;
            try
            {
                IndustryGraphDto industryGraphDto = new IndustryGraphDto();
                industryGraphDto.chart = new Chart();
                industryGraphDto.chart.time = new List<DateTime>();

                // 对应这个行业的所有数据中date从小到大的排列
                industryGraphDto.chart.time = await _db.Queryable<IndustryData>()
                .LeftJoin<Industry>((i, c) => i.code == c.code)
                .Where((i, c) => c.name == industry)
                .OrderBy(i => i.date)
                .Select(i => i.date)
                .ToListAsync();

                industryGraphDto.chart.volume = new List<long>();
                // 与上面date对应顺序的volume值（市值）
                industryGraphDto.chart.volume = await _db.Queryable<IndustryData>()
                .LeftJoin<Industry>((i, c) => i.code == c.code)
                .Where((i, c) => c.name == industry)
                .OrderBy(i => i.date)
                .Select(i => i.volume)
                .ToListAsync();

                // 用于绘制表格的数据
                industryGraphDto.table = await _db.Queryable<IndustryData>()
                .LeftJoin<Industry>((i, c) => i.code == c.code)
                .Where((i, c) => c.name == industry)
                .Select(i => new IndustryDataDto
                {
                    date = i.date,
                    code = i.code,
                    open = i.open,
                    close = i.close,
                    turnover = i.turnover,
                    totalshare = i.totalshare

                })
                .ToListAsync();

                _logger.LogInformation("获取某个行业的图表所需数据成功");
                ApiResponse response = new ApiResponse(HttpStatusCodes.OK, industryGraphDto, "获取某个行业的图表所需数据成功");
                return Ok(response);
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation(ex, "获取某个行业的图表所需数据失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取某个行业的图表所需数据失败");
                return BadRequest(response); //false表示失败
            }
        }

        //获取某个上市公司的图表所需数据 
        [HttpPost]
        public async Task<IActionResult> GetListedCompanyGraphData([FromBody] GetCompanyInfoRequest getCompanyInfoRequest)
        {
            string uid = getCompanyInfoRequest.uid;
            try
            {
                //获取该公司的股票代码
                string stock=await _db.Queryable<Company>()
                .Where(c=>c.uid==uid)
                .Select(c=>c.stock)
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

                        CompanyGraph graphData= new CompanyGraph();


                        graphData.yearList= new List<string>(companyGraphDto.yearList);
                        graphData.profits = new List<double>(companyGraphDto.profits);
                        graphData.rate = new List<double>(companyGraphDto.rate);
                        graphData.money = new List<double>(companyGraphDto.money);
                        graphData.debt = new List<double>(companyGraphDto.debt);
                        graphData.debt_rate = new List<double>(companyGraphDto.debt_rate);
                        _logger.LogInformation("获取某个上市公司的图表所需数据成功");

                        ApiResponse response = new ApiResponse(HttpStatusCodes.OK, graphData, "获取某个上市公司的图表所需数据成功");
                        return Ok(response);
                    }
                    else
                    {
                        _logger.LogError("调用 Python 接口失败: " + responseMessage.StatusCode);
                        ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取图表数据失败，调用Python服务异常");
                        return BadRequest(response);
                    }
                }
            }
            catch (System.Exception ex)
            {
                _logger.LogInformation(ex, "获取某个上市公司的图表所需数据失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取某个上市公司的图表所需数据失败");
                return BadRequest(response); //false表示失败
            }
        }
    }
}
