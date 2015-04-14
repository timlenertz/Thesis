scatterplot(pcf::exper::results("output/hilo.db").query("SELECT make_loose_arg, final_actual_error FROM run WHERE make_loose_arg<0.99 AND make_loose_arg>0.01"))
