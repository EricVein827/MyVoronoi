#include <igl/readOBJ.h>
#include "MyVoronoi.h"


int main(int argc, char* argv[])
{
    const string fname = (argc > 1) ? argv[1] : CGAL::data_file_path(INPUT_DIR + "fusee.obj");
    MXd V;
    MXi F;
    igl::read_triangle_mesh(fname, V, F);v
  
    MyVoronoi VD(V);

	return 0;
}
