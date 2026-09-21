# 正式UE工程保留位置

正式工程统一放在`game/EchoPlayRTT/`，由程序从`work/program`建立并提交项目设置后，各工种再导入资源。本次只建立目录约定，没有生成虚假的UE工程、项目文件或资产。

当前可行性原型仍在`prototypes/showdown-mode-concept/`，不移动、不覆盖、不把美术生产资源混入丢弃型原型。

计划目录：

```text
game/EchoPlayRTT/
  EchoPlayRTT.uproject     # 后续由程序实际创建
  Config/                 # 程序统一维护
  Source/                 # 程序
  Content/EchoPlay/
    Core/                 # 玩法蓝图、AI、UI逻辑
    Art3D/                # 3D场景、角色、动画；工种独立预览场景
    VFX/                  # Niagara、专属材质、预览场景
    UI/Art/               # UI纹理、图标、头像
    Shared/               # 共用骨架/母材质，由指定接入者维护
    Maps/                 # 主关卡，由关卡负责人维护
```

这些目录以.gitkeep占位，不能直接在UE中启动。工程生成后，保留现有工种路径，避免再次搬迁导致引用变化。没有正式工程时，先交`assets/source/`源文件与交付说明即可。

策划中的逻辑ID与UE资源名分开：`CHR_A`可对应`SK_CHR_A`，`ENV_Well_Base`对应`SM_ENV_Well_Base`，`AN_Common_Run`对应`AS_Common_Run`，`SK_A_FLUTE`是技能ID而非骨骼网格前缀。遵循现有`.claude/docs/technical-preferences.md`的UE命名约定。
