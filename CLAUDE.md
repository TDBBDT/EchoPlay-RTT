# 江湖暗局 — 项目说明

RTT（即时战术）潜行游戏。核心参考：赏金奇兵3（Desperados III）。
详细概念见 `design/gdd/game-concept.md`。

## Technology Stack

- **Engine**: Unreal Engine 5.6
- **Language**: Blueprint (primary — gameplay, level scripting, AI behavior), C++ (critical path — vision cone, Showdown Mode, patrol AI)
- **Build System**: Unreal Build Tool (UBT)
- **Asset Pipeline**: Unreal Content Pipeline

## Engine Version Reference

@docs/engine-reference/unreal/VERSION.md

## 技术偏好

@.claude/docs/technical-preferences.md

## 项目原则

- **先复刻，后创新** — 完整复刻赏金奇兵3 的成熟系统，只做已批准的微创新
- **MVP 优先** — MVP 未通过验证就不扩展范围
- **最大风险是决战时刻（Showdown Mode）** — UE 无成熟参考方案

## 回复语言

中文。
