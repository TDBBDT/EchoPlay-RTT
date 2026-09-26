# EchoPlay · RTT 战术潜行项目

EchoPlay是一支7人团队，正在制作以中国山林为背景的即时战术潜行游戏。玩家通过观察视野、诱敌、藏尸、环境交互和角色配合推进关卡。

仓库保存设计文档、制作规范、美术交付约定与UE 5.6技术原型。**当前处于设计与原型验证阶段；图文设计完成不代表关卡已在引擎中实现。**《林间无声》、岚刃、苇音均为制作方案中的工作名。

## 最新图文方案：山寺暗渡

**[打开山寺首关图文入口](design/gamejam-21d/levels/mountain-temple/README.md)** · **[阅读完整关卡设计](design/gamejam-21d/levels/mountain-temple/林间无声_山寺首关设计.md)**

![山寺暗渡整体样貌](design/gamejam-21d/levels/mountain-temple/概念图/03_统一布局全景概念.png)

> 2026-09-26 · 设计提案v2.0。全景为AI辅助制作的美术概念；精确空间、人数和高差以统一坐标图与设计表为准。

本关从岚刃单人潜入开始：完成首杀后，观察被双哨覆盖的大门，沿藤蔓下到崖底，经过同高巨石水平跳跃，再从庭院背后上墙。处理独哨、推落巨石解决谈话组后，从侧门会合苇音，进入双人教学与综合解题。

| 内容 | 本次设计 |
|---|---|
| 教学主线 | 首杀 → 下藤与平跳 → 三哨庭院 → 苇音会合 → 三信任务 → 双人撤离 |
| 敌人配置 | 首敌1名、庭院3名、河岸8名、终段4名，共16名 |
| 双人目标 | 击杀并搜索3名持信敌人；其余敌人可按所选解法处理或绕开 |
| 空间规则 | 崖下H−3、庭院H0、落石台H+3；水平跳跃与藤蔓升降分开 |
| 环境交互 | 庭院落石、河岸牛后踢；配套替代解与失败恢复 |
| 图文交付 | 9张图纸，各含PNG与SVG；整体概念、机制资产概念、完整设计稿、坐标与检查数据 |
| 验证状态 | 59项静态检查通过；实际导航、巡逻时序、镜头与通关体验待UE灰盒验证 |

![首关教学流程](design/gamejam-21d/levels/mountain-temple/图纸/01_教学流程动线图.png)

本提案预计首次游玩22—30分钟，扩展了原制作包“7敌、单文书、10—15分钟”的首关范围。两版关系与工期调整见[首关入口的版本说明](design/gamejam-21d/levels/mountain-temple/README.md#版本与执行口径)。

## 按工作内容进入

| 要做什么 | 入口 |
|---|---|
| 看最新关卡流程、点位、视锥与高差 | [山寺首关图文目录](design/gamejam-21d/levels/mountain-temple/README.md) |
| 查角色技能、视野、敌人身份与操作规则 | [01程序执行策划](design/gamejam-21d/01_程序执行策划.md) → [07程序补充](design/gamejam-21d/07_程序补充_视野警戒与敌人身份.md) |
| 按工种领取制作任务 | [21天Game Jam制作包](design/gamejam-21d/README.md) |
| 提交资源、选分支或合并 | [团队提交指南](CONTRIBUTING.md) |
| 交美术源文件与引擎资源 | [美术入口](assets/README.md) · [资源交付模板](production/templates/asset-handoff.md) |
| 建立正式UE工程 | [game目录约定](game/README.md) |
| 阅读早期设计与技术验证 | [原始游戏概念](design/gdd/game-concept.md) · [决战时刻原型](prototypes/showdown-mode-concept/README.md) |
| 查引擎版本和技术规范 | [UE版本参考](docs/engine-reference/unreal/VERSION.md) · [项目约定](CLAUDE.md) |

## 团队与分支

| 分支 | 负责工作 | 主要目录 |
|---|---|---|
| `work/design` | 玩法／关卡策划、文案、设计交付 | `design/`、`production/`、主关卡 |
| `work/program` | 程序、技术策划、系统与原型 | `game/`、`prototypes/` |
| `work/art-3d` | 3D场景、角色、动画 | `assets/source/3d/` |
| `work/art-2d` | 2D美术、UI | `assets/source/2d/` |
| `work/vfx` | 特效与战术反馈 | `assets/source/vfx/` |
| `develop` | 跨工种集成与试玩 | 合并通过审核的工作分支 |
| `main` | 队长维护的稳定快照 | 接收经集成验收的develop |

协作顺序：**工作分支 → PR到develop → 集成试玩 → main**。本次山寺图文归属`work/design`；概念图放在设计目录，正式可导入的美术资产仍按各工种源文件规范交付。主地图与共用骨架由指定负责人维护，详细规则见[CONTRIBUTING.md](CONTRIBUTING.md)。

## 仓库结构

```text
design/
├── gamejam-21d/                   # 00—07制作文档与旧版离线手册
│   └── levels/mountain-temple/    # 山寺首关：设计稿、9张图纸、概念图与坐标
├── gdd/                          # 早期游戏概念
└── art/                          # 早期MVP美术需求
assets/                          # 分工种美术源文件
game/                            # 正式UE工程位置约定
prototypes/showdown-mode-concept/ # 决战时刻可行性原型
docs/engine-reference/unreal/     # UE版本、风险与参考
production/                      # 排期、资源交付、认领与会话记录
tools/check_repository.py         # 仓库文件与LFS检查
```

## 获取图片与离线阅读

GitHub中直接阅读Markdown，图纸使用PNG预览，另有SVG供无损放大。图片遵循仓库的Git LFS规则：

```bash
git lfs install
git clone --branch work/design https://github.com/TDBBDT/EchoPlay-RTT.git
cd EchoPlay-RTT
git lfs pull
```

本地打开`design/gamejam-21d/levels/mountain-temple/山寺首关设计.html`即可查看带目录、图片缩放的离线图文版。HTML需和同目录的`图纸/`、`概念图/`保持在一起；GitHub文件页不会直接运行HTML。图片逐文件提交，便于版本比较，不把重复ZIP塞入仓库。

## 技术与当前阶段

- 引擎：Unreal Engine 5.6；Blueprint承担主要玩法与关卡脚本，C++处理关键系统；面向PC，目标60fps。
- 现有`showdown-mode-concept`是战术时间系统验证原型。其记录指出仅设置`CustomTimeDilation = 0`不足以冻结全部几何检测，需要显式门控；新关卡并未因此获得UE运行验证。
- 早期GDD强调暂停编排；21天方案将同步规划列为可选。山寺首关的推荐解均允许实时切人完成，原型继续保留用于技术验证。
- 下一步按设计稿搭建灰盒，优先验证大门封锁、藤蔓与跳点、八敌巡逻网和双人撤离，再接入精修美术。

## 项目背景与参考

项目是EchoPlay团队面向TapTap聚光灯游戏创作挑战与MoBius Game Jam的前期预演，工作内容以中文维护。旧概念、旧首关和技术原型保留在各自目录，供追溯比较。

山寺方案参考《影子战术：将军之刃》《赏金奇兵3》的公开开发材料，关注清晰反馈、可观察的战术区域、推荐路线与多解验证。具体来源、采用方式与本项目改编边界见[完整设计稿](design/gamejam-21d/levels/mountain-temple/林间无声_山寺首关设计.md)。
