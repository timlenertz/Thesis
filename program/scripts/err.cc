using namespace Eigen;

auto b = bunny();
auto b2 = b;
b2.downsample_random(0.05);
auto cor = make_same_point_correspondences(b, b2);

float mn_x = -0.1, mx_x = 0.1;
float mn_y = -0.1, mx_y = 0.1;

std::size_t nx = 20;
std::size_t ny = 20;
array_2dim<float> fld({ nx, ny });
for(std::ptrdiff_t xi = 0; xi < nx; ++xi) {
for(std::ptrdiff_t yi = 0; yi < ny; ++yi) {
	float x = map_num<std::ptrdiff_t, float>(xi, 0, nx, mn_x, mx_x);
	float y = map_num<std::ptrdiff_t, float>(yi, 0, ny, mn_y, mx_y);
	
	Vector3f v1(x, y, 0);

	Eigen::Translation3f t(v1);
	Eigen::Affine3f trans(t);

	mean_square_error err;
	cor(err, trans);
	
	float e  = err();
	
	fld[{ xi, yi }] = e;
}
}


auto get_err = [&](Double_t* args, Double_t*) -> Double_t {
	float x = args[0], y = args[1];
	std::ptrdiff_t xi = map_num<float, std::ptrdiff_t>(x, mn_x, mx_x, 0, nx);
	std::ptrdiff_t yi = map_num<float, std::ptrdiff_t>(y, mn_y, mx_y, 0, ny);
	return fld[{ xi, yi }];
};



TF2* f = new TF2("plot", get_err, mn_x, mx_x, mn_y, mx_y, 0, 0);

TCanvas* can = new TCanvas("c1", "Surf", 200, 10, 700, 700)
TPad* pad = new TPad("pad1", "surf", 0.03, 0.03, 0.98, 0.98)
pad->Draw()
f->Draw("cont");
