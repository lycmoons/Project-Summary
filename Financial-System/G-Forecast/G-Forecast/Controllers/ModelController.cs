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
using System.Web;
using G_Forecast.dto.model;

namespace G_Forecast.Controllers
{
    [Route("/[controller]/[action]")]
    [ApiController]
    [EnableCors("any")]
    public class ModelController : ControllerBase
    {
        private readonly SqlSugarClient _db;
        private readonly ILogger<CompanyController> _logger; // 声明日志记录器

        public ModelController(ILogger<CompanyController> logger)
        {

            _logger = logger; // 初始化日志记录器
            //获取数据库连接
            dbORM dborm = new dbORM();
            _db = dborm.getInstance();
        }

        // 获取神经网络预测所需的模版CSV文件
        //[Authorize]
        [HttpGet]
        public async Task<IActionResult> GetTemplate()
        {
            var filePath = Path.Combine(Directory.GetCurrentDirectory(), "wwwroot", "templates", "template.csv");

            if (!System.IO.File.Exists(filePath))
            {
                return NotFound("模板文件不存在");
            }

            var fileBytes = await System.IO.File.ReadAllBytesAsync(filePath);
            var fileName = "template.csv";

            // 返回文件
            return File(fileBytes, "text/csv", fileName);
        }

        //获取基于决策树的企业信用等级评估结果
        [HttpPost]
        public async Task<IActionResult> GetCredit([FromBody] DecisionTreeRequest decisionTreeDto)
        {
            using (var httpClient = new HttpClient())
            {
                // 构造查询字符串
                var query = HttpUtility.ParseQueryString(string.Empty);
                query["debt_rate"] = decisionTreeDto.debt_rate.ToString();
                query["interest"] = decisionTreeDto.interest.ToString();
                query["inventory_turn"] = decisionTreeDto.inventory_turn.ToString();
                query["flow_debt"] = decisionTreeDto.flow_debt.ToString();
                query["cost"] = decisionTreeDto.cost.ToString();
                query["owner_equity"] = decisionTreeDto.owner_equity.ToString();

                string url = $"http://113.44.84.116:8000/DecisionTree?{query}";

                try
                {
                    var urlResponse = await httpClient.GetAsync(url);
                    urlResponse.EnsureSuccessStatusCode();

                    
                    var jsonString = await urlResponse.Content.ReadAsStringAsync();

                    // 解析 JSON
                    using JsonDocument doc = JsonDocument.Parse(jsonString);

                    JsonElement mapElement = doc.RootElement.GetProperty("map");

                    string mapJson = mapElement.GetRawText();

                   //_logger.LogInformation("获取基于决策树的企业信用等级评估结果:mapJson:{mapJson}", mapJson);

                    var map = JsonConvert.DeserializeObject<MapDto>(mapJson);

                    //_logger.LogInformation("获取基于决策树的企业信用等级评估结果:map:{map}", map);
                    //CalculateAndPrintStats("debt", map.Debt);
                    //CalculateAndPrintStats("interest", map.Interest);
                    //CalculateAndPrintStats("turn", map.Turn);
                   // CalculateAndPrintStats("rate", map.Rate);
                    //CalculateAndPrintStats("profit", map.Profit);
                    //CalculateAndPrintStats("owner", map.Owner);

                    DecisionTreeResponse decisionTreeResponse = new DecisionTreeResponse();
                    decisionTreeResponse.debt_rate =CalculateStats(map.Debt);
                    decisionTreeResponse.interest = CalculateStats(map.Interest);
                    decisionTreeResponse.inventory_turn = CalculateStats(map.Turn);
                    decisionTreeResponse.flow_debt = CalculateStats(map.Rate);
                    decisionTreeResponse.cost = CalculateStats(map.Profit);
                    decisionTreeResponse.owner_equity = CalculateStats(map.Owner);
                    // 取 lst 数组第一个值
                    //int lstValue = doc.RootElement.GetProperty("lst")[0].GetInt32();

                    // 取 para 字段里的值
                    double debtRate = doc.RootElement.GetProperty("para").GetProperty("debt_rate").GetDouble();
                    double interest = doc.RootElement.GetProperty("para").GetProperty("interest").GetDouble();                   
                    double inventory_turn = doc.RootElement.GetProperty("para").GetProperty("inventory_turn").GetDouble();
                    double flow_debt = doc.RootElement.GetProperty("para").GetProperty("flow_debt").GetDouble();
                    double cost = doc.RootElement.GetProperty("para").GetProperty("cost").GetDouble();
                    double owner_equity = doc.RootElement.GetProperty("para").GetProperty("owner_equity").GetDouble();

                    // 取 ANNRes 字符串
                    //string annRes = doc.RootElement.GetProperty("ANNRes").GetString();

                    decisionTreeResponse.debt_rate.Add(debtRate);
                    decisionTreeResponse.interest.Add(interest);
                    decisionTreeResponse.inventory_turn.Add(inventory_turn);

                    decisionTreeResponse.flow_debt.Add(flow_debt);
                    decisionTreeResponse.cost.Add(cost);
                    decisionTreeResponse.owner_equity.Add(owner_equity);


                    _logger.LogInformation("获取基于决策树的企业信用等级评估结果成功:debt_rate:{debtRate}", debtRate);

                    ApiResponse response = new ApiResponse(HttpStatusCodes.OK, decisionTreeResponse, "获取基于决策树的企业信用等级评估结果成功");
                    return Ok(response);
                }
                catch (HttpRequestException ex)
                {
                    _logger.LogInformation(ex, "获取基于决策树的企业信用等级评估结果失败");
                    ApiResponse response = new ApiResponse(HttpStatusCodes.OK, null, "获取基于决策树的企业信用等级评估结果失败");
                    return Ok(response);
                }
            }
        }


        static void CalculateAndPrintStats(string name, List<double> data)
        {
            if (data == null || data.Count == 0)
            {
                Console.WriteLine($"{name}: 数据为空");
                return;
            }

            var sorted = data.OrderBy(x => x).ToList();

            double min = sorted.First();
            double max = sorted.Last();
            double q25 = Quantile(sorted, 0.25);
            double q50 = Quantile(sorted, 0.5);
            double q75 = Quantile(sorted, 0.75);

            Console.WriteLine($"{name}: min={min}, max={max}, Q1={q25}, median={q50}, Q3={q75}");
        }

        static List<double> CalculateStats(List<double> data)
        {
            if (data == null || data.Count == 0)
            {
                return new List<double> { double.NaN, double.NaN, double.NaN, double.NaN, double.NaN };
            }

            var sorted = data.OrderBy(x => x).ToList();

            double min = sorted.First();
            double max = sorted.Last();
            double q25 = Quantile(sorted, 0.25);
            double q50 = Quantile(sorted, 0.5);
            double q75 = Quantile(sorted, 0.75);

            return new List<double> { min, q25, q50, q75, max };
        }


        //计算分位数
        public static double Quantile(List<double> sortedData, double quantile)
        {
            if (sortedData == null || sortedData.Count == 0)
                throw new ArgumentException("Data list is empty");

            double pos = (sortedData.Count - 1) * quantile;
            int index = (int)pos;
            double frac = pos - index;

            if (index + 1 < sortedData.Count)
                return sortedData[index] * (1 - frac) + sortedData[index + 1] * frac;
            else
                return sortedData[index];
        }

        //获取神经网络CNN预测的结果
        [HttpPost]
        public async Task<IActionResult> GetCnnPredict(IFormFile targetFile)
        {
            if (targetFile == null || targetFile.Length == 0)
            {
                _logger.LogInformation("❌ 没有接收到文件");
                return BadRequest("❌ 没有接收到文件");
            }

            var serverUrl = "http://113.44.84.116:8001/predict";

            try
            {
                using (var client = new HttpClient())
                using (var form = new MultipartFormDataContent())
                {
                    // 将前端上传的文件内容读成 StreamContent
                    var fileContent = new StreamContent(targetFile.OpenReadStream());
                    fileContent.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue("text/csv");

                    // 添加到 form-data 请求里，"file" 这个名字要跟 Python 接口那边的 request.files['file'] 对应
                    form.Add(fileContent, "file", targetFile.FileName);

                    // 发请求到 Python 服务
                    var response = await client.PostAsync(serverUrl, form);

                    if (!response.IsSuccessStatusCode)
                    {
                        var error = await response.Content.ReadAsStringAsync();
                        return BadRequest($"❌ 调用 Python 接口失败，状态码: {response.StatusCode}, 错误: {error}");
                    }

                    var result = await response.Content.ReadAsStringAsync();
                    // 反序列化成对象
                    var predictionResult = JsonConvert.DeserializeObject<CnnPredictionResult>(result);
                    
                    // 判断 predictions 列表是否有值
                    if (predictionResult != null && predictionResult.predictions != null && predictionResult.predictions.Count > 0)
                    {
                        int firstPrediction = predictionResult.predictions[0];
                        _logger.LogInformation("获取神经网络CNN预测的结果成功，预测星级：{firstPrediction}",firstPrediction);
                        CnnResult cnnResult= new CnnResult();
                        cnnResult.star = firstPrediction;
                        ApiResponse response1 = new ApiResponse(HttpStatusCodes.OK, cnnResult, "获取神经网络CNN预测的结果成功");
                        return Ok(response1);
                        
                    }
                    else
                    {
                        _logger.LogInformation( "获取神经网络CNN预测的结果为空");
                        ApiResponse response1 = new ApiResponse(HttpStatusCodes.OK, null, "获取神经网络CNN预测的结果为空");
                        return Ok(response1);
                    }                           
                }
            }
            catch (Exception ex)
            {
                _logger.LogInformation(ex, "获取神经网络CNN预测的结果失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取神经网络CNN预测的结果失败");
                return BadRequest(response);
            }
        }

        //获取财务风险评估结果
        [HttpPost]
        public async Task<IActionResult> GetFinancialRisk([FromBody] FinancialInput financialInput)
        {
            try
            {
                // 检查数据是否合法
                if (financialInput == null || financialInput.input == null || financialInput.input.Count != 11)
                {
                    return BadRequest("必须传入 11 个财务指标。");
                }

                // 将对象序列化成 JSON 字符串
                var json = System.Text.Json.JsonSerializer.Serialize(financialInput);

                // 创建 HttpClient 实例
                using (var client = new HttpClient())
                {
                    var content = new StringContent(json, System.Text.Encoding.UTF8, "application/json");

                    // 发送 POST 请求到 Python 服务
                    var response = await client.PostAsync("http://113.44.84.116:5001/predict", content);

                    if (response.IsSuccessStatusCode)
                    {
                        var resultJson = await response.Content.ReadAsStringAsync();

                        // 反序列化 JSON 到 FinancialResult 对象
                        var result = System.Text.Json.JsonSerializer.Deserialize<FinancialResult>(resultJson);

                        // 解析 secondaryIndex 的四个值
                        var secondaryValues = result.secondaryIndex
                            .Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries)
                            .Select(s => double.Parse(s))
                            .ToList();

                        // 把 firstlyIndex 转成 double
                        double firstlyValue = double.Parse(result.firstlyIndex);

                        // 合并成一个 list
                        var allValues = new List<double> { firstlyValue };
                        allValues.AddRange(secondaryValues);

                        FinancialRisk financialRisk = new FinancialRisk();
                        financialRisk.o1 = allValues[0];
                        financialRisk.o2 = allValues[1];
                        financialRisk.o3 = allValues[2];
                        financialRisk.o4 = allValues[3];
                        financialRisk.o5 = allValues[4];

                        _logger.LogInformation("获取财务风险评估结果成功");
                        ApiResponse response1 = new ApiResponse(HttpStatusCodes.OK, financialRisk, "获取财务风险评估结果成功");
                        return Ok(response1);
                        
                    }
                    else
                    {
                        _logger.LogInformation("获取财务风险评估结果调用python服务失败");
                        ApiResponse response1 = new ApiResponse(HttpStatusCodes.OK, null, "获取财务风险评估结果调用python服务失败");
                        return Ok(response1);
                    }
                }
            }
            catch (Exception ex)
            {
                _logger.LogInformation(ex, "获取财务风险评估结果失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取财务风险评估结果失败");
                return BadRequest(response);
            }
        }


        //获取经营风险评估结果
        [HttpPost]
        public async Task<IActionResult> GetBusinessRisk([FromBody] FinancialInput financialInput)
        {
            try
            {
                // 检查数据是否合法
                if (financialInput == null || financialInput.input == null || financialInput.input.Count != 14)
                {
                    return BadRequest("必须传入 14 个财务指标。");
                }

                // 将对象序列化成 JSON 字符串
                var json = System.Text.Json.JsonSerializer.Serialize(financialInput);

                // 创建 HttpClient 实例
                using (var client = new HttpClient())
                {
                    var content = new StringContent(json, System.Text.Encoding.UTF8, "application/json");

                    // 发送 POST 请求到 Python 服务
                    var response = await client.PostAsync("http://113.44.84.116:5002/predict", content);

                    if (response.IsSuccessStatusCode)
                    {
                        var resultJson = await response.Content.ReadAsStringAsync();

                        // 反序列化 JSON 到 FinancialResult 对象
                        var result = System.Text.Json.JsonSerializer.Deserialize<FinancialResult>(resultJson);

                        // 解析 secondaryIndex 的四个值
                        var secondaryValues = result.secondaryIndex
                            .Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries)
                            .Select(s => double.Parse(s))
                            .ToList();

                        // 把 firstlyIndex 转成 double
                        double firstlyValue = double.Parse(result.firstlyIndex);

                        // 合并成一个 list
                        var allValues = new List<double> { firstlyValue };
                        allValues.AddRange(secondaryValues);

                        BusinessRisk businessRisk = new BusinessRisk();
                        businessRisk.o1 = allValues[0];
                        businessRisk.o2 = allValues[1];
                        businessRisk.o3 = allValues[2];
                        businessRisk.o4 = allValues[3];

                        _logger.LogInformation("获取经营风险评估结果成功");
                        ApiResponse response1 = new ApiResponse(HttpStatusCodes.OK, businessRisk, "获取经营风险评估结果成功");
                        return Ok(response1);

                    }
                    else
                    {
                        _logger.LogInformation("获取经营风险评估结果调用python服务失败");
                        ApiResponse response1 = new ApiResponse(HttpStatusCodes.OK, null, "获取经营风险评估结果调用python服务失败");
                        return Ok(response1);
                    }
                }
            }
            catch (Exception ex)
            {
                _logger.LogInformation(ex, "获取经营风险评估结果失败");
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "获取经营风险评估结果失败");
                return BadRequest(response);
            }
        }

    }
}
