#pragma once

#include <memory>
#include <unordered_map>
#include "pointcloud.h"
#include "nanoflann.hpp"

using namespace std;
using namespace nanoflann;

// kd-tree�̃C���f�b�N�X�^��`:
typedef KDTreeSingleIndexDynamicAdaptor<
	L2_Simple_Adaptor<double, PointCloud >,
	PointCloud, 2
> my_kd_tree_t;

// KDTree �N���X
class KDTree {
public:
	
	KDTree();
	KDTree(const vector<PointCloud_Point> & pts);
	~KDTree();

	void build_index(const vector<PointCloud_Point> & pts);							// kd-tree �C���f�b�N�X�̍\�z
	PointCloud_Point & get_point(const size_t id) ;	//���W�f�[�^�̎擾
	size_t add_point(const size_t id, const double x, const double y); //���W�f�[�^�̒ǉ�

	vector<PointCloud_Point>  radius_search(	//���a�����ɂ�錟��
		const PointCloud_Point & pt,
		const double distance
	); 



private:
	PointCloud m_point_cloud; //���W�f�[�^�Q
	unordered_map<size_t, size_t> m_id_map; //�C���f�b�N�X
	unique_ptr<my_kd_tree_t> m_kdtree; //kd-tree�̃|�C���^ 
};


