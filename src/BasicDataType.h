#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_3.h>
#include <CGAL/Regular_triangulation_euclidean_traits_3.h>
#include <CGAL/Regular_triangulation_vertex_base_3.h>
#include <CGAL/Regular_triangulation_cell_base_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Point_set_3.h>

using MXf = Eigen::MatrixXf;
using MXi = Eigen::MatrixXi;
using M4f = Eigen::Matrix4f;
using VXf = Eigen::VectorXf;
using VXi = Eigen::VectorXi;
using V3f = Eigen::Vector3f;
using V3i = Eigen::Vector3i;
using V2f = Eigen::Vector2f;
using V2i = Eigen::Vector2i;
using SpMatF = Eigen::SparseMatrix<float>;
using TripF = Eigen::Triplet<float>;
using PFF = std::pair<V3f, V3f>;

using MXd = Eigen::MatrixXd;
using M4d = Eigen::Matrix4d;
using VXd = Eigen::VectorXd;
using V3d = Eigen::Vector3d;
using V2d = Eigen::Vector2d;
using SpMatD = Eigen::SparseMatrix<double>;
using TripD = Eigen::Triplet<double>;
using PDD = std::pair<V3d, V3d>;

using MXV = Eigen::Matrix<V3f, Eigen::Dynamic, Eigen::Dynamic>;

using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::max;
using std::min;
using std::string;
using std::cerr;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Weighted_point_3 weightedPoint;
typedef K::Point_3 Point;
typedef K::Segment_3 Segment;
typedef K::Ray_3 Ray;
typedef CGAL::Bbox_3 Bbox;
typedef CGAL::Point_set_3<Point> Point_set;

typedef K::Triangle_3 Triangle;
typedef CGAL::Regular_triangulation_euclidean_traits_3<K> Traits;
typedef CGAL::Regular_triangulation_3<Traits> Triangulation;
typedef CGAL::Delaunay_triangulation_3<K> Delaunay;

const string OUT_DIR = "..//output//";
const string INPUT_DIR = "..//model//";
