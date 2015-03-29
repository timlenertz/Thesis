auto q = "SELECT ABS(loose_modifier_arg - fixed_modifier_arg), final_actual_error FROM run";
scatterplot("test", pcf::exper::results("output/projdown.db").query(q))
