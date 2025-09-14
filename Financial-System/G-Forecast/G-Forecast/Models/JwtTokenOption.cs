/// <summary>
/// 用来保存jwt的配置信息
/// </summary>
public class JwtTokenOption
{
    /// <summary>
    /// Token发布者
    /// </summary>
    public string Issuer { get; set; }
    /// <summary>
    /// oken接受者
    /// </summary>
    public string Audience { get; set; }
    /// <summary>
    /// 秘钥
    /// </summary>
    public string IssuerSigningKey { get; set; }
    /// <summary>
    /// 过期时间
    /// </summary>
    public int AccessTokenExpiresMinutes { get; set; }
}
