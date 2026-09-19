# Unreal Engine 5.6 — 废弃 API 对照表

> **最后验证：** 2026-09-19
> **风险等级：** MEDIUM — 本表基于 5.6 官方 API 文档与社区报告整理，非官方完整清单。
> 遇到未列入的 API 时，用 WebSearch 验证当前版本的正确用法。

## C++ 核心

| 不要用（已废弃） | 改用 | 说明 |
|------------------|------|------|
| `UClass::ClassDefaultObject` | `GetDefault<>()` / `GetMutableDefault<>()` | 全局模板函数，CDO 访问的标准方式 |
| `GMalloc` 直接访问 | 通过分配器接口访问 | 全局内存分配器访问已废弃 |
| `FString::Appendf(fmt, ...)` 动态格式串 | 格式串必须是 `static constexpr` | 5.6 加入编译期格式串检查，动态构造的格式串不再编译 |
| `UDEPRECATED_MetaData` | `FMetaData` | `FMetaData` 始终存在且由 `UPackage` 持有 |

## Slate / UMG

| 不要用（已废弃） | 改用 | 说明 |
|------------------|------|------|
| Slate 槽位偏移用 `FVector2D` | `SlotOffset2f` / `SlotSize2f` | 5.6 起 Slate API 不再直接对接双精度标量与向量 |
| `UText3DComponent::Text` 直接赋值（蓝图可写） | 调用 `SetText` UFUNCTION | 5.6 撤销了 `Text` 的蓝图可写性。相关议题：UE-308024 |

> `UText3DComponent::Text` 的变更源于 `Getter`/`Setter` 说明符误用（应为 `BlueprintGetter`/`BlueprintSetter`）。若升级旧蓝图资产时遇到 "Text3DComponent.Text is not blueprint writable" 编译错误，改调 `SetText`。

## 动画 / Control Rig

| 不要用（已废弃） | 改用 | 说明 |
|------------------|------|------|
| `RigUnit_MathQuaternionFromRotator` | 使用转换节点（cast node） | 大量 `RigUnit_Math` 节点已迁移为 `FRigVMFunction_Math` 风格 |
| `RigUnit_MathQuaternionToRotator` | 使用转换节点（cast node） | Rotator 本身未废弃 |

## 已知回归问题（5.6，尚未修复）

| 问题 | 议题编号 | 状态 |
|------|----------|------|
| `StateTreePropertyRef` 无法提升为参数 | UE-352719 | 回归缺陷。`FStateTreeBindingExtension::GetPromotionToParameterOverrideInternal` 中 `bOutOverride` 恒为 false。未进 5.7 修复清单，可能以 hotfix 形式发布 |
| GraphEditor 废弃委托包装器崩溃 | CL 41643395 | `SGraphEditor::UpgradeDeprecatedDelegates` 中 lambda 包装器在栈上 `FGraphEditorEvents` 提前析构后形成 use-after-free。已确认，目标为 5.6 hotfix |

## 本项目注意事项

- **StateTree** — 若用 StateTree 实现巡逻 AI 行为树，避开 `StateTreePropertyRef` 提升为参数的用法（UE-352719）
- **视野锥实现** — 优先用 C++ + 材质组合，避免依赖 5.6 中变更过的 Slate 双精度接口
