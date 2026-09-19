# Unreal Engine 5.5 → 5.6 — 破坏性变更

> **最后验证：** 2026-09-19
> **适用范围：** 本项目为**全新项目**（无 5.5 存量资产），因此大部分迁移类问题不适用。
> 本表主要用于：使用第三方插件时的兼容性排查，以及从教程/示例代码复制 5.5 写法时的检查。

## 官方渠道说明

Epic 未发布独立的「5.5 → 5.6 迁移指南」文档。5.6 的变更记录分散在：
- 5.6 Release Notes（官方）
- 各子系统 API 文档页面
- Epic Developer Community 论坛的 bug 报告与官方回复

## 链接与导出

### `_API` 宏从类型级移至方法级

变更号：CL 41869343

Epic 依据 Fortnite 客户端的实际使用情况，脚本化地将 `_API` 导出宏从类/结构体级别下移到单个方法级别。**本意是修复 inline/dllstorage 问题，不应移除任何导出**，但实际造成部分插件代码的链接错误。

**排查方式：** 使用第三方插件时遇到 `LNK2019` 未解析符号，且符号在 5.5 中可正常使用 → 高度怀疑此处。
**解决：** 从源码重新编译插件；或在插件源码中显式补全 `_API` 宏。Epic 已请求社区上报非预期移除的案例。

## 蓝图 / 资产

### `UText3DComponent::Text` 蓝图可写性撤销

5.5 中可直接在蓝图中设置 `Text3DComponent.Text`。5.6 中该属性不再蓝图可写，必须调用 `SetText` UFUNCTION。

**症状：** 蓝图编译报错 `Text3DComponent.Text is not blueprint writable. Set Text`
**议题：** UE-308024（源于 `Getter`/`Setter` 说明符误用，应为 `BlueprintGetter`/`BlueprintSetter`）

### Nanite 骨骼网格体崩溃

从 5.6 之前的版本转换而来的 Nanite 骨骼网格体可能崩溃。

**解决：** 回退为非 Nanite 骨骼网格体，在已转换的 5.6 项目中重新应用 Nanite。

**本项目相关性：** 角色模型若使用 Nanite 骨骼网格体，从商城资产引入时需注意其来源版本。

### 数据资产在连续迁移后崩溃

症状：`property 'UIData' belongs to class 'REINST_BP_DA_CharacterData_C_385'` → `ObjectValuePtr.IsResolved()` → `EXCEPTION_ACCESS_VIOLATION`

**成因：** 连续多次版本迁移可能损坏数据资产。
**排查：** 以 `-NoCrashDialog -log` 启动，检查 `Saved/Logs/` 中的 `PropertyObject` / `Crash Loading` 条目；移动/删除/重建故障资产；执行 Fix Up Redirectors；重编译蓝图。

**对 GAS 项目（Lyra 等）的额外步骤：** 删除 `Binaries`、`Intermediate`、`.vs`，重新生成项目文件，确认 Game Features 插件已启用，重编译游戏模块，对受影响蓝图执行 Clean and Regenerate，禁用循环蓝图。

## MetaHuman（5.6 重大变更）

若本项目使用 MetaHuman，注意 5.6 的架构调整：

| 5.5 及以前 | 5.6 |
|------------|-----|
| MetaHuman Creator 网页应用 | 编辑器内 **MetaHuman Character 资产** |
| 独立 Capture Manager 工具 | Live Link Hub 中的 Capture Manager |
| MetaHuman Animator 需下载 | 随 5.6 内置，启用插件即可；深度数据处理需另装插件 |
| Quixel Bridge 独立版用于 DCC 导出 | 改用 **DCC Export** 装配管线 |
| UEFN Importer | **UEFN Export** 装配管线 |

**兼容性约束：** MetaHuman 资产与引擎版本强绑定（5.6 资产需 5.6+）。可只升级项目而保留旧 MetaHuman 特性，但**不能只升级 MetaHuman 而不升级项目**。

## 已知问题

| 问题 | 症状 | 解决 |
|------|------|------|
| `GetEnumeratorNameAsString` 节点 | `FBlueprintEditorUtils::FindBlueprintForNodeChecked(...) failed to find a Blueprint` | 升级前移除该节点，或用自定义事件占位替代 |
| 动画重定向 | 5.6 中重定向普遍存在问题 | 在 5.5 项目中完成重定向，导出 FBX，再导入 5.6 |

## 稳定性建议（社区共识）

UE 5.6 Preview 期间的社区建议是：**除非项目急需 5.6 特性，否则留在 5.5**。理由：
- 5.5 被认为是 UE5 系列中首个稳定的 RC，PSO 缓存问题已修复
- 社区认为 5.6 在 SM6 方向的推进可能在较老 GPU 上带来性能问题
  （注：这是对该论坛帖的转述。**5.6 新建项目仍默认 SM5**，SM6 需手动启用——见 `current-best-practices.md`）
- 大量商城插件在 5.6 初期的兼容性滞后

**本项目的取舍：** 已确认使用 5.6。保持默认的 SM5 即可规避大部分性能风险；仅当引入 Nanite 资产时才需启用 SM6。若遇到性能问题，优先调整 RHI 设置，而非回退引擎版本。

**小技巧：** 若想恢复 5.6 中已移除的旧视口工具栏，使用控制台命令 `ToolMenuViewportToolbars 0`。
