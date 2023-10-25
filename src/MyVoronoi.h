# pragma once

#include "BasicDataType.h"

class MyVoronoi
{
protected:

	vector<Point> m_sites;                                 // the input sampling points (sites)

	map<Delaunay::Cell_handle, int> m_tetra_id_map;         // asign int number to each tetrahedron 

	Delaunay m_T;                                          // the Delaunay tetra

	Bbox m_bbox;                                           // the bounding box of the sampling points

	MXd m_cellVertex;                                      // the vertex of cells

	vector<V2i> m_cellEdge;

	vector<vector<int>> m_cellFacet;

public:

	MyVoronoi() {};

	MyVoronoi(const MXd& samplingPoints);

	~MyVoronoi() {};

	// add corners of a huge enough cube to sampling points, such that the boundign sampling points 
	// are located in finite cells
	void addCornerPoints(const MXd& pointCloud, const double& scale = 4);

	// set the bounding box of the sampling points
	void setBoundingBox(const MXd& pointCloud, const double& scale = 0.5);

	// asign id to each finite tetrahedron constructed by Delaunay triangulation
	void asignID2tetra();

	// get vetices of each cells, if the tetra is infinite, we set the vertex to a bounding box facet, which we defined before. 
	void getCellVertex();

	// get the edge of Voronoi diagram
	void getCellEdge();

	// get the facet of VD
	void getCellFacet();

	// only save the sites as .obj file
	void saveFile2OBJ(const string& fileName, const MXd& circumcenter) const;
	void saveFile2OBJ(const string& fileName, const vector<Point>& circumcenter) const;

	// save sites and cell edges as .obj file
	void saveVoronoiFacet2OBJ(const string& fileName) const;
};

