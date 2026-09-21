# EchoPlay 团队提交指南

## 先选分支，再放到自己的目录

每条分支都是同一个项目的完整副本，不是互相隔离的网盘。**目录负责划分资源归属，分支负责隔离未验收的改动。** 不要删除别的工种目录，也不要把完整UE工程复制一份给每个工种。

| 分支 | 使用者 | 主要负责的目录 | PR目标 |
|---|---|---|---|
| `main` | 队长发布 | 已验收的稳定快照；当前仍是前期工程，不代表已可玩 | 接收develop |
| `develop` | 队长+程序集成 | 每天集成、试玩、修复 | main |
| `work/program` | 程序+技术策划 | `game/EchoPlayRTT/Source/`、`Config/`、`Content/EchoPlay/Core/`，现有`prototypes/` | develop |
| `work/art-3d` | 3D+动画 | `assets/source/3d/`、`game/EchoPlayRTT/Content/EchoPlay/Art3D/`、`production/handoffs/3d/` | develop |
| `work/vfx` | 特效 | `assets/source/vfx/`、`game/EchoPlayRTT/Content/EchoPlay/VFX/`、`production/handoffs/vfx/` | develop |
| `work/art-2d` | 2D/UI | `assets/source/2d/`、`game/EchoPlayRTT/Content/EchoPlay/UI/Art/`、`production/handoffs/2d/` | develop |
| `work/design` | 队长/关卡/文案 | `design/`、`production/`、`game/EchoPlayRTT/Content/EchoPlay/Maps/` | develop |

`Shared/`共用骨架、母材质、公共数据由技术策划/程序指定一名负责人处理；UI逻辑和WBP放在`Core/UI/`，2D负责`UI/Art/`的图像，避免两个人同时改一个控件蓝图。

同一岗位多人时，从最新develop开短期任务分支，如`feat/program-vision`或`asset/3d-well`，完成后直接PR到develop；不要为了“回到部门分支”多做一次合并。部门分支适合一个主要提交者，短期分支适合多人并行。

## 第一次开始

安装Git和Git LFS。推荐本地克隆后提交，不用网页上传大型模型/UE资产：网页上传不会替你执行本地LFS过滤器。

```bash
git lfs install
git clone https://github.com/TDBBDT/EchoPlay-RTT.git
cd EchoPlay-RTT
git switch --track origin/work/art-3d
git lfs pull
```

把示例分支换成自己的工种。若本地分支已存在，使用`git switch work/art-3d`。每个成员需有仓库写入权限；本次只建立协作结构，没有代替队长邀请成员。

## 每天开始制作

先保存并提交手头工作，工作区干净后：

```bash
git switch work/art-3d
git fetch origin
git merge --ff-only origin/work/art-3d
git merge origin/develop
git lfs pull
```

`--ff-only`失败说明该工作分支已有他人的新提交或历史分叉，停止并与同分支成员协调；用普通merge解决，不强推。`git merge origin/develop`发生冲突时，按下方规则处理，也可以`git merge --abort`回到合并前。

共享长期分支不用rebase，不执行force push；各部门至少每天同步develop一次。

## 提交一个可验收的小批次

例如交一口井，而不是把一周所有东西塞进一个压缩包：

1. 源文件放`assets/source/3d/environment/ENV_Well/`，FBX与贴图放其`export/`子目录。
2. 若已建立正式UE工程，把导入后的资产放`Content/EchoPlay/Art3D/Environment/Well/`。工程未建立时交源文件即可，不虚构.uasset。
3. 复制[交付模板](production/templates/asset-handoff.md)到`production/handoffs/3d/ENV_Well.md`，填写比例、贴图、碰撞、引擎依赖、预览和验收。
4. 仅暂存本次相关文件，检查后提交：

```bash
git add assets/source/3d/environment/ENV_Well production/handoffs/3d/ENV_Well.md
git diff --cached --stat
git lfs status
git commit -m "asset(3d): add well model and handoff"
git push -u origin work/art-3d
```

若同时提交UE导入资产，额外`git add`对应的Well目录。不要直接`git add .`把编辑器生成文件混进来。

5. 打开GitHub，创建Pull Request，**base选develop，compare选自己的工作分支**。默认分支是main，建PR时需要主动改base。
6. 按PR模板填写文件、依赖和验收。只有在目标引擎中导入/打开过才勾“已接入”，只交模型不能写“已可玩”。

常用入口：[程序](https://github.com/TDBBDT/EchoPlay-RTT/compare/develop...work/program?expand=1) · [3D](https://github.com/TDBBDT/EchoPlay-RTT/compare/develop...work/art-3d?expand=1) · [特效](https://github.com/TDBBDT/EchoPlay-RTT/compare/develop...work/vfx?expand=1) · [2D](https://github.com/TDBBDT/EchoPlay-RTT/compare/develop...work/art-2d?expand=1) · [策划](https://github.com/TDBBDT/EchoPlay-RTT/compare/develop...work/design?expand=1)。刚建立时各分支相同，尚无可比较改动是正常现象。

## 队长如何合并

工作分支 → PR → develop → 集成试玩 → main。小批次每天合一次，不等比赛最后一天集中合。

- 长期部门分支的PR使用 **Create a merge commit**，不要squash/rebase merge；这样保留共同祖先，下次该分支继续交付不会反复出现已合入提交。
- 每次合入后，作者将最新develop merge回自己的工作分支，再继续制作。长期工作分支不自动删除。
- 短期任务分支可以squash，合完即删除并从最新develop重新开；不要在已squash的旧任务分支上继续累计后续工作。
- 共用骨架/公共材质/输入接口先合，再合依赖这些资源的角色/技能，最后由主关卡负责人接入场景。
- develop至少完成一次集成打开和关键路径检查后，队长创建develop→main的PR，用merge commit合入；随后把main同步回develop。
- 打包结果上传Release或团队分发位置，不把整个打包目录、DDC或Saved塞进Git。

## 二进制资源如何避免冲突

LFS负责大文件存储，**不会把两个人修改的同一个.uasset/.umap/.blend自动拼起来**。同一个二进制文件一次只允许一名负责人修改。

1. 在`production/claims/`为该资产复制[认领模板](production/templates/asset-claim.md)，先同步到develop；不同资产一份文件，避免所有人争写一张总表。
2. 对已经提交的共用二进制文件尝试LFS锁：

```bash
git lfs lock "game/EchoPlayRTT/Content/EchoPlay/Shared/Characters/SKEL_Common.uasset"
git lfs locks
```

这只是示例路径，文件尚未创建时不要原样执行。Git LFS锁定适用于仓库中同一路径的所有分支；如果服务端权限/锁不可用，以认领记录和指定负责人约束制作。本次没有声称已经开启GitHub强制锁规则。

3. PR合入后、下一个人开始前执行`git lfs unlock "实际文件路径"`。
4. 真发生冲突时，双方保留各自源文件，由负责人选定一个版本并在UE中重新导入/重做另一方改动；不要把二进制冲突一键“全部采用我的”后当成整合成功。

主地图只由关卡负责人编辑；3D/特效可在自己的`Art3D/Preview/`、`VFX/Preview/`放独立预览图，不动主地图。公共骨架、物理资产、动画蓝图、母材质、项目设置、输入配置都按同样方式指定单人维护。

在UE编辑器内移动/改名资产，并由接入者检查Redirector与引用；不要在资源管理器里随手移动.uasset。参考[Epic Redirectors说明](https://dev.epicgames.com/documentation/en-us/unreal-engine/asset-redirectors-in-unreal-engine)。

## LFS和自动检查

`.gitattributes`已声明UE、模型、贴图、音频及美术源文件的LFS规则。小型JSON、文档、代码保持普通Git文本。不要把FBX、贴图都打成ZIP提交，团队需要逐文件审查与更新。

LFS使用单独存储与下载额度，实际额度取决于仓库账户；当前没有上传新的美术大文件，也没有购买配额。[GitHub LFS说明](https://docs.github.com/en/repositories/working-with-files/managing-large-files/about-git-large-file-storage)

仓库检查会验证：应走LFS的文件是否为指针、是否误交生成目录、是否夹带超大普通Git文件。它不验证UE编译、资产引用、艺术质量或服务端LFS对象完整性；接入者仍需`git lfs pull`并在UE中验证。PR模板包含对应步骤。

当前目录归属是协作约定，不是GitHub目录级写权限。没有成员账号清单，因此没有配置虚假的CODEOWNERS；服务器分支保护/必需审核也未在本次设置。队长可随后在仓库设置对main/develop启用PR审核、禁强推和通过检查要求。
