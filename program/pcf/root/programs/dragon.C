using namespace pcf;

ply_importer imp("../data/dragon.ply");
unorganized_point_cloud_xyz pc(imp);
vw->add_point_cloud(pc);
