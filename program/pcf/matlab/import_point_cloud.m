sz = load("size.dat");
disp("Size:")
disp(sz)
fflush(stdout);


function pc = load_point_cloud_dat(name, type, sz)
	disp(name)
	fflush(stdout);

	file = fopen(name, 'r');
	pc = fread(file, sz, type);
	fclose(file);
	pc = flipud(pc);
end

v = load_point_cloud_dat("v.dat", 'uint8', sz);
az = load_point_cloud_dat("az.dat", 'float32', sz);
el = load_point_cloud_dat("el.dat", 'float32', sz);
r = load_point_cloud_dat("r.dat", 'float32', sz);
x = load_point_cloud_dat("x.dat", 'float32', sz);
y = load_point_cloud_dat("y.dat", 'float32', sz);
z = load_point_cloud_dat("z.dat", 'float32', sz);

az(v == false) = NaN;
el(v == false) = NaN;
r(v == false) = NaN;
x(v == false) = NaN;
y(v == false) = NaN;
z(v == false) = NaN;