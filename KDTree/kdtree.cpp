#include "kdtree.h"
#include <iostream>
// �R���X�g���N�^
KDTree::KDTree() 
{
	build_index({});
}
// �R���X�g���N�^
KDTree::KDTree(const vector<PointCloud_Point>& pts)
{	
	build_index(pts);

}
// �R���X�g���N�^
KDTree::~KDTree() = default;
// kd-tree �C���f�b�N�X�̍\�z
void KDTree::build_index(const vector<PointCloud_Point>& pts)
{
	// ���W�f�[�^��ޔ�
	// �f�[�^�̃N���A
	m_id_map.clear();
	m_point_cloud.pts.clear();
	// kd-tree�̏�����
	m_kdtree.reset(new my_kd_tree_t(2, m_point_cloud, KDTreeSingleIndexAdaptorParams()));

	if (pts.size() > 0)
	{
		//���W�f�[�^�̒ǉ�������
		for (const PointCloud_Point& it : pts)
		{
			m_id_map[it.id] = m_point_cloud.pts.size();
			m_point_cloud.pts.push_back(it);
		}
		//kd-tree�̍č\�z
		m_kdtree->addPoints(0, m_point_cloud.pts.size() - 1);
	}
}
//���W�f�[�^�̎擾
PointCloud_Point & KDTree::get_point(const size_t id)  
{

	const auto iter = m_id_map.find(id);
#ifdef DEBUG
	if (iter != m_id_map.end())	cout << "ID: %d does not exists", id); << endl;
#endif // DEBUG
	size_t i = iter->second;
#ifdef DEBUG
	if (i < m_point_cloud.pts.size())	cout << "Index out bounds, index:", i << endl;
#endif // DEBUG
	return  m_point_cloud.pts[i];
}
//���W�f�[�^�̒ǉ�
size_t KDTree::add_point(const size_t id, const double x, const double y)
{
	const size_t last = m_point_cloud.pts.size();

	m_point_cloud.pts.emplace_back(id, x, y);	
	m_kdtree->addPoints(last, last);
	m_id_map[id] = last;
	return last;
}
// //���a�����ɂ�錟��
vector<PointCloud_Point> KDTree::radius_search(const PointCloud_Point &pt, const double distance)
{
	// �p�����[�^�̐ݒ�
	const double query_pt[2]{ pt.x, pt.y };
	nanoflann::SearchParams params;
	params.sorted = false;
	// �������ʏo�̓p�����[�^�̐ݒu
	vector<pair<size_t, double>> indices_dists;
	RadiusResultSet<double, size_t> resultSet(distance, indices_dists);
	// ����
	m_kdtree->findNeighbors(resultSet, query_pt, params);

	// ���W���X�g�ɕϊ�
	int i = 0;
	vector<PointCloud_Point> result(resultSet.size());
	for (const pair<size_t, double>& it : resultSet.m_indices_dists)
	{
		result[i++] = m_point_cloud.pts[it.first];
	}
	return move(result);

}
