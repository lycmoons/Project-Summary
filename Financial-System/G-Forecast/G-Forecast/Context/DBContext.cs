using System;
using System.Data;
using MySqlConnector;  // MySQL 数据库连接库
using SqlSugar;

namespace G_Forecast
{
    public class dbORM   // 数据库ORM类，用于配置和获取数据库实例
    {
        public SqlSugarClient getInstance() //sqlsugar是一种ORM框架
        {
            SqlSugarClient db = new SqlSugarClient(connect_info);   // 创建SqlSugarClient对象并返回
            return db;
        }

        public static ConnectionConfig connect_info = new ConnectionConfig()  // 数据库连接配置信息
        {
            ConnectionString = "Server=localhost;Port=3306;Database=g-forecast;User=root;Password=123456;", // MySQL连接串
            DbType = SqlSugar.DbType.MySql, // 指定数据库类型为 MySQL
            IsAutoCloseConnection = true,  // 自动关闭连接
            InitKeyType = SqlSugar.InitKeyType.Attribute  // 初始化键类型为属性
        };
    }

    public class UserDB
    {
        // 数据库连接字符串
        public static string connString = "Server=100.80.103.167;Port=3306;Database=mydb;User=root;Password=yourpassword;";
        public MySqlConnection con;  // MySQL 数据库连接对象

        // 连接到 MySQL 数据库的方法
        public bool connectMySQL()
        {
            try
            {
                // 建立 MySqlConnection 对象
                con = new MySqlConnection(connString);
                // 打开连接
                con.Open();
                return true; // 如果连接成功，返回true

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);  // 输出错误信息
                return false;   // 如果连接失败，返回false
            }
        }
    }
}
