using System;
using System.Runtime.InteropServices;

public static class ControllerWrapper
{
    // 指定 DLL 文件的路径（可以用相对路径，或者把 DLL 放在 C# 项目 `bin` 目录）
    private const string DllName = "testuse.dll"; // 确保 controller.dll 在可执行目录下

    // 运动控制指令组
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int controlTopDiskRotation(int diskNumber);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int controlBottomDiskRotation(int diskNumber);

    // 状态读取指令组
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int readTopDiskInfo(int order, out long getRspData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int readBottomDiskInfo(int order, out long getRspData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int readfocusInfo(int order, out long getRspData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int readapertureInfo(int order, out long getRspData);

    // 精密运动控制指令组
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int controlapertureRotation(long value);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int controlfocusRotation(long value);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int testa_add(int a, int b);
}
