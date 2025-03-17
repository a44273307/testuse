#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

#ifdef _WIN32
#ifdef CONTROLLER_EXPORTS
#define CONTROLLER_API __declspec(dllexport)
#else
#define CONTROLLER_API __declspec(dllimport)
#endif
#else
#define CONTROLLER_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // 运动控制指令组 ---------------------------------------------------

    /**
     * @brief 控制上圆盘旋转到指定孔位
     * @param diskNumber 目标孔位编号（有效范围：1-8）
     * @return 参见sendorderandcheckrsp返回值
     */
    CONTROLLER_API int controlTopDiskRotation(int diskNumber);

    /**
     * @brief 控制下圆盘旋转到指定孔位
     * @param diskNumber 目标孔位编号（有效范围：1-8）
     * @return 参见sendorderandcheckrsp返回值
     */
    CONTROLLER_API int controlBottomDiskRotation(int diskNumber);

    // 状态读取指令组 ---------------------------------------------------

    /**
     * @brief 读取上圆盘状态信息
     * @param order 查询指令类型：
 *              - 02: 实时位置
 *              - 03: 目标位置
 *              - 04: 复位完成标志
 *              - 05: 运行电流
 * @param getRspData 输出参数，解析后的状态数据
 * @return 参见sendorderandrspans返回值
     */
    CONTROLLER_API int readTopDiskInfo(int order, long long* getRspData);

    /**
     * @brief 读取下圆盘状态信息（参数同上圆盘）
     */
    CONTROLLER_API int readBottomDiskInfo(int order, long long* getRspData);

    /**
     * @brief 读取聚焦机构状态信息（参数同上圆盘）
     */
    CONTROLLER_API int readfocusInfo(int order, long long* getRspData);

    /**
     * @brief 读取光圈机构状态信息（参数同上圆盘）
     */
    CONTROLLER_API int readapertureInfo(int order, long long* getRspData);

    // 精密运动控制指令组 ------------------------------------------------

    /**
     * @brief 控制光圈位置
     * @param value 目标位置（有效范围：0-1500000）
     * @return 参见sendorderandcheckrsp返回值
     */
    CONTROLLER_API int controlapertureRotation(long long value);

    /**
     * @brief 控制聚焦位置
     * @param value 目标位置（有效范围：0-1500000）
     * @return 参见sendorderandcheckrsp返回值
     */
    CONTROLLER_API int controlfocusRotation(long long value);

    // 测试函数
    CONTROLLER_API int testa_add(int a,int b);

#ifdef __cplusplus
}
#endif

#endif // CONTROLLER_H
