# EchoPlay · RTT 战术潜行项目

> **工作名（TBD）** — 游戏正式名称尚未确定，本仓库暂以玩法类型 RTT 指代项目。
> 这是一款以「观察 → 暂停编排 → 同时执行」为核心的**即时战术潜行**游戏。

---

## 这是什么

本仓库是 **EchoPlay** 团队 RTT 战术潜行项目的**前期工作区**：设计文档、美术需求、
技术约定，以及在 Unreal Engine 5.6 中验证核心机制可行性的原型代码。

项目目前处于 **预演（Pre-production / Rehearsal）阶段**，目标不是尽快交付一个
能玩的成品，而是回答两个问题：

1. 这套「观察–暂停–编排–执行」的循环，在中国背景下能不能让人上瘾？
2. 这支队伍能不能按预想的节奏，把一个想法推到可玩？

因此仓库里的内容是「正在发生」的：文档会改，原型会被推倒重来，结论也可能是
「这条路走不通」。我们把这些过程都留在版本历史里。

---

## 比赛背景

本项目是 **TapTap 聚光灯游戏创作挑战** 与 **MoBius Game Jam** 的一次**项目预演**。

在正式参赛之前，我们希望先用一段完整的前期开发流程 —— 玩法概念 → 技术验证 →
可玩原型 —— 跑通团队协作，摸清我们的产能边界、引擎风险与决策节奏。
这次预演产出的全部设计文档、技术结论与原型代码都在这个仓库里。

---

## 团队：EchoPlay

一支 7 人小队，策划、程序、美术全栈覆盖，从概念到原型在同一条流水线上。

| 岗位 | 分工 |
|------|------|
| **技术策划** | 系统设计、技术方案选型、引擎能力验证 |
| **程序** | 客户端与引擎开发、核心系统实现、工具链与构建 |
| **玩法 / 关卡策划（队长）** | 核心循环与关卡设计、玩法验证、进度把控 |
| **文案策划** | 世界观与人物设定、任务文本、叙事包装 |
| **特效** | 技能与打击反馈、战术可视化表现 |
| **3D 美术** | 场景与角色资产、模型与材质 |
| **2D 美术 / UI** | 界面与图标、战术 UI 信息层级、宣传物料 |

> 我们是一支「策划先想清楚、程序先证明能跑、美术再做漂亮」的队伍。
> 预演阶段刻意保持小规模与短决策链 —— 谁的问题谁拍板，一天内出结论。

---

## 玩法一句话

> 你不是一个莽夫，而是一个**棋手**。战场上每一个敌人、每一段巡逻路线、
> 每一处阴影都是棋盘上的变量。

玩家操控一支各怀绝技的小队潜行渗透，在敌人视野与巡逻的缝隙里规划路线，
用**信息优势**和**执行精度**以少胜多。战斗靠的是脑子，不是手速。

### 核心循环（约 30 秒）

```
观察视野锥 → 暂停布置指令 → 恢复时间同时执行 → 处理尸体 → 推进
```

- **视野锥实时可见** —— 信息完全对称，没有隐藏机制，失败必须能归因
- **暂停即思考** —— 时间冻结，玩家编排多个角色的一串指令
- **同时执行** —— 恢复时间的瞬间，精心编排的动作同时展开，这是标志性手感
- **清场不是结束** —— 尸体能不能藏住，才是持续的压力来源

### 设计支柱

| 支柱 | 含义 |
|------|------|
| **精密编排** | 每一次清场都应该像一首指挥好的乐曲，而不是一场混战 |
| **角色即工具箱** | 每个角色都是不可替代的工具，队伍组合决定可能性空间 |
| **关卡即谜题** | 每关都有多条解法，但每条都需要思考，没有无脑路线 |

### 我们不做的事

同样重要的是边界：**不做随机生成关卡**、**不做实时动作战斗**、
**不做复杂叙事系统**、**不做开放世界**、**不做多人联机**、
**不做技能树与数值成长**。

每一个「不做」都对应着一条会被破坏的支柱 —— 权力应该来自玩家理解，
而不是角色等级或反应速度。

---

## 当前进展

### ✅ 已完成

- **游戏概念文档 v1.0** —— 核心身份、支柱与反支柱、核心循环、MDA 分析、
  视觉身份锚点、机制清单、MVP 范围与风险评估
- **MVP 美术需求清单** —— 资产范围与优先级
- **技术约定** —— 命名规范、性能预算、资产前缀、测试策略
- **UE 5.6 引擎参考** —— 版本风险、已知破坏性变更与最佳实践

### 🚧 进行中：核心机制可行性原型

`prototypes/showdown-mode-concept/` 是一个**丢弃型原型**，只回答一个问题：

> UE 5.6 能否实现「暂停编排、恢复后帧精确按序执行」的战术时间系统？

选择它作为第一个验证目标，是因为它是整个项目**唯一没有成熟引擎参考方案**的
高风险系统 —— 它做不出来，后面的一切都没有意义。

原型零二进制资产依赖：全部场景由 GameMode 在运行时生成，全部画面用调试绘制，
不需要蓝图、不需要导入资源，打开空关卡就能跑。

**已经得到的关键结论：** 单靠 `CustomTimeDilation = 0` **不足以**冻结世界 ——
它只能停住读取 `DeltaTime` 的逻辑，而视野检测这类纯几何运算仍然会继续运行，
必须显式门控。这条结论会直接带进正式设计。

---

## 仓库结构

```
.
├── CLAUDE.md                          # 项目速览与协作约定
├── design/
│   ├── gdd/game-concept.md            # 游戏概念文档 v1.0
│   └── art/mvp-asset-requirements.md  # MVP 美术资产需求
├── docs/
│   └── engine-reference/unreal/       # UE 5.6 版本、破坏性变更与最佳实践
├── production/
│   └── session-state/active.md        # 当前工作会话状态
├── prototypes/
│   └── showdown-mode-concept/         # 战术时间系统可行性原型（UE 5.6 / C++）
└── .claude/docs/technical-preferences.md
```

---

## 技术栈

| 项目 | 选择 |
|------|------|
| **引擎** | Unreal Engine 5.6 |
| **语言** | Blueprint（主力：玩法、关卡脚本、AI 行为）<br>C++（关键路径：视野判定、战术时间系统、巡逻 AI） |
| **构建** | Unreal Build Tool |
| **目标平台** | PC / Steam |
| **性能目标** | 60 fps（16.6 ms 帧预算） |
| **输入** | 键鼠为主，手柄做到「可玩」 |

---

## 下一步

1. 完成战术时间系统的原型试玩验证，输出结论（继续 / 转向 / 终止）
2. 若核心机制成立 → 确认视觉身份，进入 MVP 开发
3. 若核心机制不成立 → 回到概念阶段，重新评估方向

MVP 的目标不是「做一个能玩的游戏」，而是**验证核心循环是否好玩**：
测试者失败后会不会主动再试一次、会不会为了规划而停下来思考、
会不会主动使用时间暂停机制。

---

## 关于本项目的说明

- 本仓库当前内容以**中文**为主。
- 原型目录下的代码是**验证性代码**，明确不作为正式实现的基础，会被重写。
- 项目设计参考了成熟 RTT 作品（如 *Desperados III*）的系统框架，
  方法论是「先复刻成熟系统，再做极少量微创新」—— 我们不想把预算花在
  赌一个未经市场验证的自创机制上。

---

<details>
<summary><b>English (brief)</b></summary>

**EchoPlay — RTT Tactical Stealth Project (working title)**

We are a 7-person team building a real-time tactics stealth game in Unreal Engine 5.6.
The core loop is *observe → pause and plan → execute simultaneously*: you command a
small squad of specialists, and success comes from information advantage and
execution precision rather than reflexes.

This repository is a **rehearsal run** for the **TapTap Spotlight Game Creation
Challenge** and the **MoBius Game Jam** — a full pre-production pass (concept →
technical validation → playable prototype) taken before we enter the competitions
proper. It currently holds our design documentation, technical conventions, and a
throwaway Unreal prototype validating the highest-risk system: frame-accurate
time-stop command execution.

</details>
