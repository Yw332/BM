# blueprint_apply_graph

<!-- Written by Unreal Box from real engine results. Edit or delete freely; statistics live in .ledger.json. -->

## VariableGet 引用了不存在的变量
<!-- id: e-064d2b35 -->
- key: blueprint_apply_graph · "variable not found: linecolor"
- advice: VariableGet 节点的 member_name 必须是该蓝图上已存在的变量。先用 blueprint_get_graph 查看现有变量，或改用常量/函数节点提供颜色值。
- expect: tool=blueprint_get_graph
- source: 2026-10-28 · session 2be2ffeb · failed 2x, then succeeded
- verified: 2026-10-28 · UE 5.4.4-35576357+++UE5+Release-5.4
- status: trial
