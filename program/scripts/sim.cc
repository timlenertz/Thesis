using namespace Eigen;

pose ps1 = pose::from_string("0.0111054,-5.34705,3.16065,0.888177,0.459471,0.00140555,0.00514437");
pose ps2 = pose::from_string("1.19905,5.00239,3.73934,0.0271785,0.0955291,0.430536,0.897092");
pose vps = pose::from_string("-3.91371,0.746101,4.11871,0.631202,0.178814,-0.261022,-0.70815");

projection_image_camera cam1(ps1, projection_frustum::symmetric_perspective_fov_x(60.0, 1.0), 10000, 10000);
projection_image_camera cam2(ps2, projection_frustum::symmetric_perspective_fov_x(60.0, 1.0), 1500, 1500);

unorganized_point_cloud_full og = make_relief_point_cloud(5.0, 50000, 1);
og.set_point_indices();
bounding_box og_b = og.box();

auto fx_w = og, lo_w = std::move(og);

bounding_box fx_b = og_b, lo_b = og_b;
//fx_b.origin += Vector3f(0.0, 0.0, 0);
//fx_b.extremity -= Vector3f(0.3, 0.3, 0);
//lo_b.origin += Vector3f(0.3, 0.3, 0);
//lo_b.extremity -= Vector3f(1.0, 2.0, 0);

fx_w.filter(crop_point_filter<bounding_box>(fx_b));
lo_w.filter(crop_point_filter<bounding_box>(lo_b));

set_unique_color(lo_w.begin(), lo_w.end(), rgb_color::white);
set_unique_color(fx_w.begin(), fx_w.end(), rgb_color::red);

fx_w.downsample_projection(cam1);
lo_w.downsample_projection(cam2);

unorganized_point_cloud_full lo = std::move(lo_w);
kdtree_point_cloud_full fx = std::move(fx_w);

viewer_window vw;
auto& sfx = vw->add(fx);
auto& slo = vw->add(lo);
vw.select_object(lo);
vw->camera().set_relative_pose(vps);

auto cor = make_same_point_correspondences(fx, lo);
cor.mode = decltype(cor)::known_transformation;
cor.real_loose_transformation = Eigen::Affine3f::Identity();
auto reg = iterative_correspondences_registration<decltype(cor)>(cor);
reg.maximal_iterations = -1;
reg.run_live(fx, lo);
