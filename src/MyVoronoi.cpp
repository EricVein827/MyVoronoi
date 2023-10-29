// MyVoronoi.cpp: 定义应用程序的入口点。
//
#include "MyVoronoi.h"

MyVoronoi::MyVoronoi(const MXd& samplingPoints)
{
    // tranverse the data to vector<Point>
    //m_sites.resize(samplingPoints.rows());

    saveFile2OBJ(OUT_DIR + "sampling points.obj", samplingPoints);

    for (int i = 0; i < samplingPoints.rows(); i++)
    {
        V3d p = samplingPoints.row(i);
        m_sites.emplace_back(Point(p.x(), p.y(), p.z()));
    }

    addCornerPoints(samplingPoints);

    getCellVertex();

    //getCellEdge();

    getCellFacet();

    saveVoronoiFacet2OBJ(OUT_DIR + "VD.obj");
}

void MyVoronoi::addCornerPoints(const MXd& pointCloud, const double& scale)
{
    V3d minP = pointCloud.colwise().minCoeff();
    V3d maxP = pointCloud.colwise().maxCoeff();
    double length = (maxP - minP).norm() * scale;

    double minX = minP.x() - length;
    double minY = minP.y() - length;
    double minZ = minP.z() - length;

    double maxX = maxP.x() + length;
    double maxY = maxP.y() + length;
    double maxZ = maxP.z() + length;

    double middleX = 0.5 * (minX + maxX);
    double middleY = 0.5 * (minY + maxY);
    double middleZ = 0.5 * (minZ + maxZ);

    m_sites.emplace_back(Point(minX, minY, minZ));
    m_sites.emplace_back(Point(minX, minY, maxZ));
    m_sites.emplace_back(Point(minX, maxY, minZ));
    m_sites.emplace_back(Point(minX, maxY, maxZ));
    m_sites.emplace_back(Point(maxX, minY, minZ));
    m_sites.emplace_back(Point(maxX, minY, maxZ));
    m_sites.emplace_back(Point(maxX, maxY, minZ));
    m_sites.emplace_back(Point(maxX, maxY, maxZ));

    m_sites.emplace_back(Point(middleX, minY, minZ));
    m_sites.emplace_back(Point(middleX, minY, maxZ));
    m_sites.emplace_back(Point(middleX, maxY, minZ));
    m_sites.emplace_back(Point(middleX, maxY, maxZ));
    m_sites.emplace_back(Point(minX, middleY, minZ));
    m_sites.emplace_back(Point(minX, middleY, maxZ));
    m_sites.emplace_back(Point(maxX, middleY, minZ));
    m_sites.emplace_back(Point(maxX, middleY, maxZ));
    m_sites.emplace_back(Point(minX, minY, middleZ));
    m_sites.emplace_back(Point(minX, maxY, middleZ));
    m_sites.emplace_back(Point(maxX, minY, middleZ));
    m_sites.emplace_back(Point(maxX, maxY, middleZ));

    m_sites.emplace_back(Point(middleX, middleY, minZ));
    m_sites.emplace_back(Point(middleX, middleY, maxZ));
    m_sites.emplace_back(Point(minX, middleY, middleZ));
    m_sites.emplace_back(Point(maxX, middleY, middleZ));
    m_sites.emplace_back(Point(middleX, minY, middleZ));
    m_sites.emplace_back(Point(middleX, maxY, middleZ));
}

void MyVoronoi::setBoundingBox(const MXd& pointCloud, const double& scale)
{
    V3d minP = pointCloud.colwise().minCoeff();
    V3d maxP = pointCloud.colwise().maxCoeff();
    double length = (maxP - minP).norm() * scale;

    Point_set cornerPoint;
    cornerPoint.insert(Point(minP.x() - length, minP.y() - length, minP.z() - length));
    cornerPoint.insert(Point(maxP.x() + length, maxP.y() + length, maxP.z() + length));

    m_bbox = CGAL::bbox_3(cornerPoint.points().begin(), cornerPoint.points().end());
}

void MyVoronoi::asignID2tetra()
{
    m_T.insert(m_sites.begin(), m_sites.end());

    int cellidx = 0;
    for (Delaunay::Finite_cells_iterator it = m_T.finite_cells_begin(); it != m_T.finite_cells_end(); ++it) {
        m_tetra_id_map[it] = cellidx;
        cellidx++;
    }
}

void MyVoronoi::getCellVertex()
{
    asignID2tetra();

    m_cellVertex.resize(m_tetra_id_map.size(), 3);  
    
    cout << "NUM = " << m_tetra_id_map.size() << endl;
    for (Delaunay::Finite_cells_iterator it = m_T.finite_cells_begin(); it != m_T.finite_cells_end(); ++it) 
    {
        // 访问细胞的顶点
        Delaunay::Cell_handle cell = it;
        if (it->is_valid())
            cout << m_tetra_id_map[it] << endl;
        
        Point circumcenter = m_T.dual(cell);
        std::cout << "Circumcenter: " << circumcenter << std::endl;
        
        m_cellVertex.row(m_tetra_id_map[it]) = Eigen::RowVector3d(circumcenter.x(), circumcenter.y(), circumcenter.z());
    }
}

void MyVoronoi::getCellEdge()
{
    for (Delaunay::Finite_cells_iterator it = m_T.finite_cells_begin(); it != m_T.finite_cells_end(); ++it)
    {
        // check whether the four neighboring tetras are finite, if so, then the segment connecting the corresponding 
        // circumcenters will be an edge of the VD
        for (int i = 0; i < 4; ++i) 
        {
            Delaunay::Cell_handle neighbor = it->neighbor(i);
            if (!m_T.is_infinite(neighbor)) 
            {
                // 如果邻居不是无限的，说明四面体的第i个面不是边界面
                m_cellEdge.emplace_back(V2i(m_tetra_id_map[neighbor], m_tetra_id_map[it]));
            }
        }
    }
}

void MyVoronoi::getCellFacet()
{
    for (Delaunay::Finite_edges_iterator it = m_T.finite_edges_begin(); it != m_T.finite_edges_end(); it++)
    {
        vector<int> cellID;
        // get the first incident cell of trhe edge *it
        auto cell = m_T.incident_cells(*it);
        auto cell_start = cell;
        
        do {
            //cout << "ID = " << m_tetra_id_map[cell] << endl;

            cellID.emplace_back(m_tetra_id_map[cell]);  // record the correspoding cell id

            ++cell;
        } while (cell_start != cell);

        m_cellFacet.emplace_back(cellID);
        //cout << "===== end ========" << endl;
    }
}

void MyVoronoi::saveFile2OBJ(const string& fileName, const MXd& circumcenter)const
{
    std::ofstream out;
    out.open(fileName);

    if (out.is_open()) {
        // 写入数据到文件

        for (int i = 0; i < circumcenter.rows(); i++)
        {
            Eigen::RowVector3d p = circumcenter.row(i);
            out << "v " << p.x() << " " << p.y() << " " << p.z() << endl;
        }
       
        // 关闭文件
        out.close();
        cout << "Sampling points file written successfully." << endl;
    }
    else {
        cerr << "Failed to write the file." << endl;
    }
}

void MyVoronoi::saveFile2OBJ(const string& fileName, const vector<Point>& circumcenter)const
{
    std::ofstream out;
    out.open(fileName);

    if (out.is_open()) {
        // 写入数据到文件

        for (Point p : circumcenter)
        {
            out << "v " << p.x() << " " << p.y() << " " << p.z() << endl;
        }
        
        // 关闭文件
        out.close();
        cout << "File written successfully." << endl;
    }
    else {
        cerr << "Failed to write the file." << endl;
    }
}

void  MyVoronoi::saveVoronoiFacet2OBJ(const string& fileName) const
{
    std::ofstream out;
    out.open(fileName);

    if (out.is_open()) {
        // 写入数据到文件

        for (int i = 0; i < m_cellVertex.rows(); i++)
        {
            auto p = m_cellVertex.row(i);
            out << "v " << p.x() << " " << p.y() << " " << p.z() << endl;
        }

       /* for (int i = 0; i < m_cellEdge.size(); i++)
        {
            V2i e = m_cellEdge[i];
            out << "l " << e.x() + 1 << " " << e.y() + 1 << endl;
        }*/

        for (int i = 0; i < m_cellFacet.size(); i++)
        {
            vector<int> f = m_cellFacet[i];
            int num = f.size();

            out << "f ";
            for (int j = 0; j < num; j++)
            {
                out << f[j] + 1 << " ";
            }
            out << endl;
        }
        // 关闭文件
        out.close();
        cout << "VD file written successfully." << endl;
    }
    else {
        cerr << "Failed to write the file." << endl;
    }
}
