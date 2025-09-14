using MailKit.Net.Smtp;  //一个流行的 .NET 邮件发送库，支持 SMTP/POP/IMAP
using MailKit.Security;
using MimeKit;  //用来构建电子邮件内容（标题、正文、附件等）

namespace G_Forecast.Service
{
    public class EmailService
    {
        /*发送邮件
           toEmail：收件人邮箱地址
           subject：邮件标题
           body：邮件正文
         */
        public async Task SendEmailAsync(string toEmail, string subject, string body)
        {
            var message = new MimeMessage();
            message.From.Add(new MailboxAddress("G-Forecast验证码系统", "17815925046@163.com"));// 设置发件人,参数：显示名称、发件邮箱
            message.To.Add(new MailboxAddress("", toEmail)); //设置收件人
            message.Subject = subject;  // 设置邮件标题
            message.Body = new TextPart("plain") //设置邮件正文，TextPart("plain") 表示邮件内容是纯文本
            {
                Text = body
            };

            using (var client = new SmtpClient())//创建 SMTP 客户端对象，用来连接到邮件服务器发送邮件
            {
                await client.ConnectAsync("smtp.163.com", 465, SecureSocketOptions.SslOnConnect);  
                await client.AuthenticateAsync("17815925046@163.com", "ULtmawhiT3jcQCnk");  //邮箱和授权码
                await client.SendAsync(message);
                await client.DisconnectAsync(true);
            }
        }
    }
}
