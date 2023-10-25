#include <igl/readOBJ.h>
#include "MyVoronoi.h"


int main()
{
    MXd V;
    MXi F;
    igl::readOBJ(INPUT_DIR + "switchmec.obj", V, F);
  
    MyVoronoi VD(V);

	return 0;
}