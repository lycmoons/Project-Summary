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
using Org.BouncyCastle.Tls;


namespace G_Forecast.Controllers
{
    [Route("/[controller]/[action]")]
    [ApiController]
    [EnableCors("any")]
    public class UserController : ControllerBase
    {
        private readonly SqlSugarClient _db;
        private readonly ILogger<UserController> _logger; // 声明日志记录器
        private readonly IMemoryCache _cache;
        private readonly EmailService _emailService;
        private readonly IConfiguration _configuration;

        public UserController(ILogger<UserController> logger, IMemoryCache cache, EmailService emailService,IConfiguration configuration)
        {
            _cache = cache;
            _emailService = emailService;

            _configuration = configuration;

            _logger = logger; // 初始化日志记录器
            //获取数据库连接
            dbORM dborm = new dbORM();
            _db = dborm.getInstance();
        }

        //查询指定id的用户
        //[Authorize]
        [HttpPost]
        public async Task<IActionResult> get_by_userid([FromBody] int Id)
        {
            var currentUsername = HttpContext.User.Identity?.Name;

            User user = await _db.Queryable<User>()
                .Where(it => it.id == Id)
                .FirstAsync();

            if (user != null)
            {
                _logger.LogInformation("用户 [{Username}]成功查询指定id的用户: {@Id}", currentUsername,Id); // 记录查询成功
                ApiResponse response = new ApiResponse(HttpStatusCodes.OK,user, "成功查询指定id的用户");
                return Ok(response);
            }
            else
            {
                _logger.LogError("查询指定id的用户失败: {@Id}", Id); // 记录查询失败
                ApiResponse response = new ApiResponse(HttpStatusCodes.NotFound, null, "查询指定id的用户失败");
                return NotFound(response); 
            }            
        }

        //用户注册
        [HttpPost]
        public async Task<IActionResult> AddUser([FromBody] ReigsterDto user)
        {
             _logger.LogInformation("收到用户注册数据: {@user}", user); 

            try
            {
                // 检查是否存在相同的邮箱
                var existingPlayer = await _db.Queryable<User>()
                                              .Where(it => it.email == user.email)
                                              .FirstAsync();

                if (existingPlayer != null)
                {
                     _logger.LogWarning("重复注册，用户已存在: {@user}", user); // 记录重复注册信息
                    ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "重复注册");
                    return Ok(response);
                }
                
                var maxId = await _db.Queryable<User>()
                                           .MaxAsync(it => it.id);
                int newId;
                if (maxId == 0)
                {
                    newId = 1;
                }
                else 
                    newId = (int)maxId + 1;
                User newUser = new User();
                newUser.id = newId;
                newUser.name = user.username;
                newUser.password = user.password;
                newUser.email = user.email;

                VerifyCodeRequest verifyCodeRequest = new VerifyCodeRequest();
                verifyCodeRequest.Email = user.email;
                verifyCodeRequest.Code = user.code;

                if (VerifyCode(verifyCodeRequest))
                {
                    // 插入新选手数据
                    await _db.Insertable(newUser).ExecuteCommandAsync();

                    //检验是否能查到
                    var insertedUser = await _db.Queryable<User>().OrderByDescending(p => p.id).FirstAsync();
                    _logger.LogInformation("用户成功注册，用户id: {@insertedUser.id}", insertedUser.id);

                    ApiResponse response = new ApiResponse(HttpStatusCodes.OK, null, "注册成功");
                    return Ok(response);
                }
                else
                {
                    
                    _logger.LogInformation("注册验证码错误");

                    ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "验证码错误");
                    return Ok(response);
                }
                
            }
            catch (System.Exception ex)
            {
                _logger.LogError(ex, "用户注册失败"); // 记录错误信息
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "注册失败");
                return BadRequest(ex.Message);
            }
        }

        //用户登录       
        [HttpPost]
        public async Task<IActionResult> Login([FromBody] LoginDto loginDto)
        {
            _logger.LogInformation("收到用户登录数据: {@loginDto.email}", loginDto.email);

            try
            {
                var existingUser = await _db.Queryable<User>()
                    .Where(it => it.password == loginDto.password && it.email == loginDto.email)
                    .FirstAsync();

                if (existingUser != null)
                {
                    _logger.LogInformation("用户登录成功: {@loginDto.email}", @loginDto.email); // 记录登录成功
                    var token = GenerateToken(loginDto.email);
                    UserToken userToken = new UserToken();
                    userToken.token= token;
                    ApiResponse response = new ApiResponse(HttpStatusCodes.OK, userToken, "登录成功");
                    return Ok(response);
                }
                else
                {
                    _logger.LogWarning("登录失败: 无效的用户名或密码");
                    ApiResponse response = new ApiResponse(HttpStatusCodes.NotFound, null, "登录失败: 无效的用户名或密码");
                    return Ok(response);
                }
            }
            catch (System.Exception ex)
            {
                _logger.LogError(ex, "登录失败:{@loginDto.email}", @loginDto.email); // 记录错误信息
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "登录异常");
                return BadRequest(response);
            }
        }

        // 发送验证码
        [HttpPost]
        public async Task<IActionResult> SendCode([FromBody] EmailRequest request)
        {
            try
            {
                string email= request.Email;
                if (string.IsNullOrWhiteSpace(email))
                    return BadRequest("邮箱不能为空");

                var code = new Random().Next(100000, 999999).ToString();
                _cache.Set(email, code, TimeSpan.FromMinutes(5));

                await _emailService.SendEmailAsync(email, "G-Forecast平台注册验证码", $"你的验证码是：{code}，5分钟内有效。");

                _logger.LogInformation("成功发送验证码"); 

                ApiResponse response = new ApiResponse(HttpStatusCodes.OK, null, "验证码已发送");
                return Ok(response);
            }
            catch (System.Exception ex)
            {
                _logger.LogError(ex, "发送验证码失败"); // 记录错误信息
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "注册失败");
                return BadRequest(ex.Message);
            }
        }

        // 验证验证码接口
        [HttpPost]
        public bool VerifyCode(VerifyCodeRequest request)
        {
            if (!_cache.TryGetValue(request.Email, out string cachedCode))
            {
                //ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "验证码已过期或不存在");
                return false;
            }

            if (cachedCode != request.Code)
            {
                //ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "验证码不正确");
                return false;

            }
                
            _cache.Remove(request.Email); // 验证通过后移除
            //ApiResponse response1 = new ApiResponse(HttpStatusCodes.OK, null, "验证成功");
            return true;
        }

        //修改密码
        [HttpPost]
        public async Task<IActionResult> ModifyPassword([FromBody] ModifyPasswordDto modifyPasswordDto)
        {
            try
            {
                VerifyCodeRequest verifyCodeRequest = new VerifyCodeRequest();
                verifyCodeRequest.Email = modifyPasswordDto.Email;
                verifyCodeRequest.Code = modifyPasswordDto.Code;

                if (VerifyCode(verifyCodeRequest))
                {
                  
                    //查出原有信息
                    var user = await _db.Queryable<User>()
                        .Where(it=> it.email == modifyPasswordDto.Email)
                        .FirstAsync();
                    //修改密码
                    user.password=modifyPasswordDto.Password;

                    //更新
                    await _db.Updateable(user).ExecuteCommandAsync();
                    _logger.LogInformation("用户{@modifyPasswordDto.Email}成功修改密码",modifyPasswordDto.Email);

                    ApiResponse response = new ApiResponse(HttpStatusCodes.OK, null, "成功修改密码");
                    return Ok(response);
                }
                else
                {
                    _logger.LogInformation("修改密码注册验证码错误");
                    ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "修改密码验证码错误");
                    return Ok(response);
                }
            
            
            }
            catch (System.Exception ex)
            {
                _logger.LogError(ex, "修改密码失败"); // 记录错误信息
                ApiResponse response = new ApiResponse(HttpStatusCodes.BadRequest, null, "修改密码失败");
                return BadRequest(ex.Message);
            }
        }


        //生成token
        private string GenerateToken(string username) //要设为private，否则swagger会扫描到，没加http方法报错
        {
            var jwtSettings = _configuration.GetSection("JwtSettings");

            var claims = new[]  //配置 JWT 后，身份信息存在HttpContext.User中
            {
                new Claim(ClaimTypes.Name, username),
                new Claim(JwtRegisteredClaimNames.Jti, Guid.NewGuid().ToString())
            };

            var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(jwtSettings["SecretKey"]));
            var creds = new SigningCredentials(key, SecurityAlgorithms.HmacSha256);

            var token = new JwtSecurityToken(
                issuer: jwtSettings["Issuer"],
                audience: jwtSettings["Audience"],
                claims: claims,
                expires: DateTime.Now.AddHours(2),
                signingCredentials: creds
            );

            return new JwtSecurityTokenHandler().WriteToken(token);
        }




    }
}
