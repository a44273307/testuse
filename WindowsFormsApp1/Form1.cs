using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int sum = ControllerWrapper.testa_add(10, 20);
            
            //Console.WriteLine($"testa_add(10, 20) = {sum}");
            MessageBox.Show(sum.ToString());
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
           InitializeComponent();
           CreateControlButtons();
        }
        private void CreateControlButtons()
    {
        // 按钮配置列表（按钮名称，显示文本）
        var buttonConfigs = new List<Tuple<string, string>>() 
        {
            new Tuple<string, string>("btnControlTopDiskRotation", "控制上圆盘旋转到指定孔位"),
            new Tuple<string, string>("btnControlBottomDiskRotation", "控制下圆盘旋转到指定孔位"),
            new Tuple<string, string>("btnReadTopDiskInfo", "读取上圆盘状态信息"),
            new Tuple<string, string>("btnReadBottomDiskInfo", "读取下圆盘状态信息（参数同上圆盘）"),
            new Tuple<string, string>("btnReadfocusInfo", "读取聚焦机构状态信息（参数同上圆盘）"),
            new Tuple<string, string>("btnReadapertureInfo", "读取光圈机构状态信息（参数同上圆盘）"),
            new Tuple<string, string>("btnControlapertureRotation", "控制光圈位置"),
            new Tuple<string, string>("btnControlfocusRotation", "控制聚焦位置")
        };

        // 创建按钮并设置属性
        int verticalPosition = 10;
        foreach (var config in buttonConfigs)
        {
            var btn = new Button
            {
                Name = config.Item1,
                Text = config.Item2,
                Location = new Point(20, verticalPosition),
                Size = new Size(280, 40),
                Font = new Font("Microsoft YaHei", 10F)
            };
            btn.Click += Button_ClickHandler;
            
            Controls.Add(btn);
            verticalPosition += 50;
        }
    }

    private void Button_ClickHandler(object sender, EventArgs e)
    {
        var button = sender as Button;
        // 这里添加对应功能的实现逻辑
        MessageBox.Show($"点击按钮：{button?.Name}\n对应功能：{button?.Text}");
    }
    }
}
