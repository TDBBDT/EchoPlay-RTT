# Unreal Engine — 版本参考

| 字段 | 值 |
|------|-----|
| **引擎版本** | Unreal Engine 5.6 |
| **项目锁定日期** | 2026-09-19 |
| **文档最后验证** | 2026-09-19 |
| **LLM 知识截止** | 2025-08 |
| **风险等级** | **LOW–MEDIUM — 正式版发布早于知识截止，但发布后热修复与社区实践不在训练数据内** |

## 风险评估说明

**UE 5.6 正式版发布：2025-06-03**（Preview：2025-05-13）。

由于发布日期早于 LLM 知识截止（2025-08），应当区分两类信息：

| 信息类型 | 可靠性 | 说明 |
|----------|--------|------|
| **5.6 发布特性与主 API 面** | 较可靠 | 发布说明在训练数据内 |
| **发布后 hotfix、社区踩坑经验、插件兼容性实况** | 不可靠 | 不在训练数据内，必须查证 |

**操作规则：** 涉及具体 API 名称、函数签名、废弃标记时，先查本目录文档，再用 WebSearch 验证。

## 项目已知风险

### 着色器模型：SM5 是默认，SM6 需手动启用

> ⚠️ **更正记录（2026-09-19）：** 本文件早期版本错误地声称"5.6 默认启用 SM6"。**这是错误的。** 已更正如下。

**事实：**
- **新建 UE5 项目默认使用 SM5**（DirectX 11 & 12），以维持最大的硬件兼容性
- **SM6（DirectX 12 SM6）标记为 Experimental**，需手动启用
- 启用方式：Project Settings → Platforms → Windows → Default RHI 设为 `DirectX 12`，并在 Targeted RHIs 中勾选 `DirectX 12 (SM6)`，然后重启编辑器
- 引擎级默认值可通过修改 `Engine/Config/BaseEngine.ini` 更改（这会节省新建 `.uproject` 时的着色器编译时间），但**不建议**，因为会影响所有项目

**对本项目的真实影响：**

关键在于：**Nanite 需要 SM6。** 若资产管线引入使用 Nanite 的商城资产，就**必须**启用 SM6，此时才会暴露性能与兼容性问题。

| 情形 | 是否需 SM6 | 备注 |
|------|-----------|------|
| 纯手写模型，不用 Nanite | 否 | 保持 SM5，兼容性最好 |
| 商城资产含 Nanite 模型 | **是** | 需启用 SM6，评估性能影响 |
| 使用 Lumen 高质量模式 | 通常需要 | 评估后再定 |

**SM6 的硬性硬件限制：** 自 5.5.4 起，SM6 的 Vulkan 后端要求 `VK_EXT_mesh_shader` 扩展，导致 **GTX 20XX / RDNA2 之前的显卡无法加载项目**（例如 GTX 1070、960M）。若目标用户的硬件分布较旧，这是发行层面的风险。

**应对方式：**
- 美术资产选型时**先确认是否含 Nanite**，再决定着色器模型
- 项目初期就在目标硬件建立性能基线
- 若启用 SM6 后性能不达标，可回退 `DefaultEngine.ini` 中的 RHI 设置为 `SF_VULKAN_SM5` 或切回 DX11/DX12 SM5
- 视野锥等战术可视化优先考虑非 Nanite、非重着色器方案（材质 + 后处理）

**社区性能报告：** UE 5.6 Preview 期间有匿名论坛报告称某项目从 249 FPS 降至 60–70 FPS（SM5 → SM6，GPU 占用 95%）。
⚠️ **此数据来自单条未经验证的社区帖子，非官方基准，仅供参考。**

### _API 导出宏变更（链接错误）

5.6 将 `_API` 导出宏从**类型级别**移至**方法级别**（CL 41869343），依据是 Fortnite 客户端的实际使用情况。Epic 表示本意是修复 inline/dllstorage 问题，不应移除任何导出，但实际造成部分插件代码的链接错误。

**应对方式：** 使用第三方插件时，若遇到 `LNK2019` 未解析符号，优先怀疑此处；从源码重新编译插件，或在插件源码中显式补全导出宏。Epic 已请求社区上报非预期移除的案例。

## 版本时间线（相对 LLM 知识截止 2025-08）

| 版本 | 发布时间 | 相对知识截止 | 备注 |
|------|----------|--------------|------|
| UE 5.3 | 2023-09 | 在数据内 | 参考可靠 |
| UE 5.4 | 2024-04 | 在数据内 | 参考可靠 |
| UE 5.5 | 2024-11 | 在数据内 | 参考可靠 |
| **UE 5.6** | **2025-06-03** | **略早于截止，边界** | **当前锁定。发布特性较可靠，post-launch 信息需验证** |
| UE 5.7 | 2025 下半年 | 超出 | 需查证 |
| UE 5.8 | 2026-06 | 超出 | UE5 世代最后一个大版本 |
| UE 6 | 未发布 | — | 预计 2027 年底 Early Access |

## 官方参考

- What's New（5.6 索引页，含 Release Notes / Migration Guide 入口）：
  https://dev.epicgames.com/documentation/unreal-engine/whats-new?application_version=5.6
- UE5 迁移指南（UE4→UE5，此非 5.5→5.6 指南）：
  https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-migration-guide
- Windows 项目设置（含 Default RHI / Targeted RHIs 选项）：
  https://dev.epicgames.com/documentation/unreal-engine/windows-settings-in-the-unreal-engine-project-settings?application_version=5.6

> **注：** Epic 未发布独立的「5.5 → 5.6 迁移指南」。5.6 变更记录分散在 Release Notes、各子系统 API 文档与社区论坛中。

## 维护

运行 `/setup-engine refresh` 可检查引擎新版本与新增废弃项。
