#pragma once
// controller.h
#ifndef CONTROLLER_H
#define CONTROLLER_H




// 运动控制指令组 ---------------------------------------------------

/**
 * @brief 控制上圆盘旋转到指定孔位
 * @param diskNumber 目标孔位编号（有效范围：1-8）
 * @return 参见sendorderandcheckrsp返回值
 */
int controlTopDiskRotation(int diskNumber);

/**
 * @brief 控制下圆盘旋转到指定孔位
 * @param diskNumber 目标孔位编号（有效范围：1-8）
 * @return 参见sendorderandcheckrsp返回值
 */
int controlBottomDiskRotation(int diskNumber);

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
int readTopDiskInfo(int order, long long* getRspData);

/**
 * @brief 读取下圆盘状态信息（参数同上圆盘）
 */
int readBottomDiskInfo(int order, long long* getRspData);

/**
 * @brief 读取聚焦机构状态信息（参数同上圆盘）
 */
int readfocusInfo(int order, long long* getRspData);

/**
 * @brief 读取光圈机构状态信息（参数同上圆盘）
 */
int readapertureInfo(int order, long long* getRspData);

// 精密运动控制指令组 ------------------------------------------------

/**
 * @brief 控制光圈位置
 * @param value 目标位置（有效范围：0-1500000）
 * @return 参见sendorderandcheckrsp返回值
 */
int controlapertureRotation(long long value);

/**
 * @brief 控制聚焦位置
 * @param value 目标位置（有效范围：0-1500000）
 * @return 参见sendorderandcheckrsp返回值
 */
int controlfocusRotation(long long value);

#endif // CONTROLLER_H
