# 技术偏好

> 本项目（江湖暗局）的引擎与代码约定。所有代码生成与审查应遵循本文件。

## Engine & Language

- **Engine**: Unreal Engine 5.6
- **Language**: Blueprint (primary — gameplay, level scripting, AI behavior), C++ (critical path — vision cone, Showdown Mode, patrol AI)
- **Build System**: Unreal Build Tool (UBT)
- **Asset Pipeline**: Unreal Content Pipeline

## 命名约定

### C++

- **类**：前缀 + PascalCase — `A` 为 Actor，`U` 为 UObject，`F` 为 struct（如 `ATacticalPlayerCharacter`、`UShowdownComponent`、`FPatrolRoute`）
- **变量**：PascalCase（如 `MoveSpeed`）
- **函数**：PascalCase（如 `TakeDamage()`）
- **布尔**：`b` 前缀（如 `bIsAlive`、`bIsAlerted`）
- **文件**：与类名一致但不含前缀（如 `PlayerController.h` 对应 `APlayerController`）

### Blueprint

- **蓝图资产**：`BP_` 前缀 + PascalCase（如 `BP_Guard`、`BP_TacticalCamera`）
- **控件蓝图**：`WBP_` 前缀 + PascalCase（如 `WBP_ShowdownBar`）
- **变量**：PascalCase（与 C++ 保持一致，避免边界混淆）
- **布尔**：`b` 前缀（如 `bIsSelected`）
- **函数**：PascalCase，动词开头（如 `UpdateVisionCone()`）
- **事件**：`On` 前缀 + PascalCase（如 `OnGuardAlerted`）

### 资产前缀约定

| 类型 | 前缀 | 示例 |
|------|------|------|
| 蓝图类 | `BP_` | `BP_Guard` |
| 控件蓝图 | `WBP_` | `WBP_AbilityBar` |
| 材质 | `M_` | `M_VisionCone` |
| 材质实例 | `MI_` | `MI_VisionCone_Alert` |
| 静态网格体 | `SM_` | `SM_House_Wall_A` |
| 骨骼网格体 | `SK_` | `SK_Character_Assassin` |
| 动画序列 | `AS_` | `AS_Guard_Idle` |
| 动画蒙太奇 | `AM_` | `AM_Guard_Takedown` |
| 数据资产 | `DA_` | `DA_CharacterStats` |
| 关卡 | （无前缀） | `L_Tutorial_01` |
| 纹理 | `T_` | `T_Ground_Cobble` |

## Input & Platform

- **Target Platforms**: PC (Steam)
- **Input Methods**: Keyboard/Mouse, Gamepad
- **Primary Input**: Keyboard/Mouse
- **Gamepad Support**: Partial — 手柄可操作但非主要目标
- **Touch Support**: None
- **Platform Notes**: RTT 类型依赖精确的鼠标指向与框选操作，键鼠是核心。手柄支持做到"可玩"即可，不作为设计约束。所有 UI 需支持键盘导航。

## Performance Budgets

- **Target Framerate**: 60 fps
- **Frame Budget**: 16.6 ms
- **Draw Call Limit**: ~3000（中规模场景）
- **Notes**: 新建 UE5 项目默认使用 SM5；SM6 为 Experimental，需手动启用，且**仅在美术资产使用 Nanite 时才需要**。项目初期即需在目标硬件建立性能基线。同屏多角色 + 视野锥可视化 + 动态光照是主要压力来源；视野锥应优先考虑材质 + 后处理实现，而非逐帧几何体。

## Testing

- **Framework**: Unreal Automation Framework
- **Unit Tests**: Automation Spec（BDD 风格，`Spec` 宏）
- **Integration Tests**: Functional Testing（关卡内自动化测试 Actor）
- **Notes**: 优先覆盖核心系统的确定性行为——视野锥判定、巡逻路径计算、决战时刻指令队列的时序。这些系统的回归会直接破坏玩法。

## Forbidden Patterns

[TO BE CONFIGURED]

## Allowed Libraries

[TO BE CONFIGURED]

> **护栏：** 不要预先添加推测性的依赖。只有当某个库/插件在本项目中**正在被集成**时才加入此列表。

## Engine Specialists

- **Primary**: unreal-specialist
- **Language/Code Specialist**: ue-blueprint-specialist (Blueprint graphs) or unreal-specialist (C++)
- **Shader Specialist**: unreal-specialist (no dedicated shader specialist — primary covers materials)
- **UI Specialist**: ue-umg-specialist (UMG widgets, CommonUI, input routing, widget styling)
- **Additional Specialists**: ue-gas-specialist (Gameplay Ability System, attributes, gameplay effects), ue-replication-specialist (property replication, RPCs, client prediction, netcode)
- **Routing Notes**: Invoke primary for C++ architecture and broad engine decisions. Invoke Blueprint specialist for Blueprint graph architecture and BP/C++ boundary design. Invoke GAS specialist for all ability and attribute code. Invoke replication specialist for any multiplayer or networked systems. Invoke UMG specialist for all UI implementation. **本项目为单人游戏——replication specialist 通常不适用。**

### File Extension Routing

| File Extension / Type | Specialist to Spawn |
|-----------------------|---------------------|
| Game code (.cpp, .h files) | unreal-specialist |
| Shader / material files (.usf, .ush, Material assets) | unreal-specialist |
| UI / screen files (.umg, UMG Widget Blueprints) | ue-umg-specialist |
| Scene / prefab / level files (.umap, .uasset) | unreal-specialist |
| Native extension / plugin files (Plugin .uplugin, modules) | unreal-specialist |
| Blueprint graphs (.uasset BP classes) | ue-blueprint-specialist |
| General architecture review | unreal-specialist |
