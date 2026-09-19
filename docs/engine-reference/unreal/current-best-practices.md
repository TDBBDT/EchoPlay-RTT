# Unreal Engine 5.6 — 当前最佳实践

> **最后验证：** 2026-09-19
> **风险等级：** MEDIUM。本文件记录 5.6 的新实践与已知陷阱。

## 项目配置

### 着色器模型：SM5 是默认，SM6 需手动启用

**新建 UE5 项目默认使用 SM5。** SM6 标记为 Experimental，需手动启用：
Project Settings → Platforms → Windows → Default RHI 设为 `DirectX 12`，Targeted RHIs 中勾选 `DirectX 12 (SM6)`，重启编辑器。

**决定是否启用 SM6 的唯一因素是：美术资产是否使用 Nanite。** Nanite 需要 SM6。

| 情形 | 需 SM6？ |
|------|---------|
| 纯手写模型，不用 Nanite | 否 — 保持 SM5 |
| 商城资产含 Nanite 模型 | **是** |

**硬件限制：** 自 5.5.4 起 SM6 的 Vulkan 后端要求 `VK_EXT_mesh_shader`，GTX 20XX / RDNA2 之前的显卡无法加载项目。

**建议流程：**
1. 美术资产选型时**先确认是否含 Nanite**
2. 项目初期就在目标硬件上建立性能基线（用真实关卡，不是空场景）
3. 若启用 SM6 后达不到 60fps，先试切换 RHI 而非其他优化
4. 不要等到项目后期才发现问题

**RTT 项目的具体压力点：** 多角色同屏 + 视野锥可视化 + 动态光照。三者叠加时优先优化视野锥（改用材质 + 后处理而非逐帧几何体）。

## 编译期检查

### 格式串必须是编译期常量

5.6 对 `FString::Appendf` 及相关函数加入格式串编译期检查。格式串必须是 `static constexpr`。

```cpp
// 5.5 可编译，5.6 报错
FString Msg;
FString Fmt = TEXT("Guard %s alerted");
Msg.Appendf(Fmt, *GuardName);

// 5.6 正确写法
Msg.Appendf(TEXT("Guard %s alerted"), *GuardName);
```

**影响：** 从旧教程或示例复制的动态格式串代码会编译失败。

## 类默认对象（CDO）访问

`UClass::ClassDefaultObject` 已废弃。使用全局模板函数：

```cpp
// 已废弃
const AActor* CDO = MyClass->ClassDefaultObject;

// 推荐
const AActor* CDO = GetDefault<AActor>(MyClass);
AActor* MutableCDO = GetMutableDefault<AActor>(MyClass);
```

## Blueprint 与 C++ 边界

本项目采用 **Blueprint 为主 + C++ 关键路径** 的分工。5.6 下的一些实践要点：

- **C++ 暴露给蓝图** — 用 `UFUNCTION(BlueprintCallable)` / `UPROPERTY(BlueprintReadWrite)`。注意 5.6 中 `Getter`/`Setter` 不再隐含蓝图可写性，必须显式写 `BlueprintGetter`/`BlueprintSetter`（参见 `UText3DComponent::Text` 的 UE-308024）
- **性能关键路径放 C++** — 视野锥射线检测、决战时刻指令队列、巡逻 AI 的路径计算
- **调参接口留蓝图** — 巡逻速度、视野角度、警戒阈值等设计师参数

**本项目建议的 C++ 优先项：**
| 系统 | 理由 |
|------|------|
| 视野锥 / 视线检测 | 每帧多次射线检测，蓝图无法承受 |
| 决战时刻指令队列 | 需要精确的时序控制与状态回滚 |
| 巡逻 AI 核心 | 确定性要求高，且需要设计师可调的路径系统 |

## 链接错误排查

遇到 5.6 独有的 `LNK2019` 未解析符号时，优先怀疑 `_API` 宏从类型级移至方法级（CL 41869343）导致的导出缺失。

**排查顺序：**
1. 确认该符号在 5.5 中是否可用
2. 若可用 → 从源码重新编译插件
3. 若插件无可编译源码 → 向插件作者报告，或联系 Epic（Epic 已请求社区上报非预期移除案例）

## 待验证项

以下内容来自 5.6 发布说明，但未在本项目上下文中验证。使用前应实测：

- **Mesh Terrain（实验性）** — 支持真正的 3D 地形（悬垂、浮空岛、隧道）。RTT 游戏通常不需要，但若关卡设计涉及多层地形可评估
- **MCP 插件（实验性）** — 允许 LLM 直接连接 UE 项目。项目开发流程如需 AI 辅助关卡设计可评估

## 维护说明

本文件的内容会随引擎版本演进而过时。运行 `/setup-engine refresh` 更新。
