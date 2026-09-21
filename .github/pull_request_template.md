## 本次交付

- 工种 / 任务ID：
- 具体改动：
- 交付清单路径（资源提交必填）：
- 依赖PR/提交（没有写无）：

## 接入与验证

- [ ] 本PR目标是develop；若为发布集成，目标是main
- [ ] 文件位于本工种目录；跨目录或公共资源修改已与负责人协调
- [ ] 新增二进制已由LFS管理，`git lfs status`已检查
- [ ] 没有提交Binaries / Intermediate / Saved / DDC / 打包文件
- [ ] 相关模型、贴图、材质、骨架和依赖资产一起交付
- [ ] 接入人在`git lfs pull`后已用UE 5.6打开/导入并检查引用（仅文档可写不适用）
- [ ] 玩法改动验证了相关功能；只交资源没有宣称功能已实现
- [ ] 已提供预览/复现步骤和已知问题

验证说明：

## 合并方式

长期work/*→develop、develop→main使用Create a merge commit；合并后工作分支同步develop。短期任务分支squash后不要继续复用。
