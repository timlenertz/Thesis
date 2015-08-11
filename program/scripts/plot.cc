scatterplot(pcf::exper::results("results/projdown/projdown.db").query("SELECT ABS(make_fixed_arg - make_loose_arg), final_actual_error FROM run"), "relief_hardMIN")


scatterplot(pcf::exper::results("results/projdown/projdown.db").query("SELECT fixed_modifier_arg, final_actual_error FROM run"), "relief_hardMIN")


scatterplot(pcf::exper::results("output/sphere.db").query("SELECT MAX(loose_number_of_points, fixed_number_of_points), final_actual_error FROM run"), "relief_hardMIN")


scatterplot(pcf::exper::results("output/icp_ex_relief/results.db").query("SELECT step, actual_error FROM state"), "relief_hardMIN")
