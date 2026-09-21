# 美术源文件入口

| 工种 | 分支 | 源文件目录 | 交付说明目录 |
|---|---|---|---|
| 3D场景、角色、动画 | work/art-3d | source/3d/ | ../production/handoffs/3d/ |
| 2D、UI | work/art-2d | source/2d/ | ../production/handoffs/2d/ |
| 特效 | work/vfx | source/vfx/ | ../production/handoffs/vfx/ |

推荐每个资产一个目录：`source/3d/environment/ENV_Well/`，内含源文件、`export/`和小型预览。固定资产名持续更新，用Git提交保留版本；不要把final、final2、最终版全留在正式目录。

UE导入资产放`game/EchoPlayRTT/Content/EchoPlay/`相应工种目录。源文件和引擎资产都要有，前者用于返修，后者用于集成；尚未导入时在交付说明中写明。

共享依赖不重复拷贝进每个工种文件夹，引用Shared目录中的统一资产。新资产先读[团队指南](../CONTRIBUTING.md)与[21天制作包](../design/gamejam-21d/README.md)。
