AABoxKDTree2dNode的构造方法：

原型：AABoxKDTree2dNode(const std::vector<ObjectPtr> &objects,
                    const AABoxKDTreeParams &params, int depth)
输入：包含对象指针的vector，参数param，当前深度depth
输出：
1.	检查vector是否为空。
2.	根据vector中所有对象的最小外接矩形的x、y轴范围，计算出所要分割的平面矩形的x、y轴范围和该平面矩形在x、y方向上的中轴。
3.	选取x、y轴范围中跨度最大的轴，作为本次平面分割选用的轴方向。
4.	检查是否满足分割的条件：（1）当前深度小于参数param中的最大深度，（2）vector中的对象数量大于参数param中的最大子叶数，（3）所要分割的x、y轴范围大于参数param中的最大叶子维度。
5.	如果不满足4的分割条件，则跳到步骤6。如果满足，就把vector中的所有完全位于分割轴左侧的对象划分到左子vector中，把所有完全位于分割轴右侧的对象划分到右子vector中，把所有与分割轴相交的对象调用InitObjects函数进行处理；同时分别对左右子vector递归调用子kdtree构造函数，用于生成当前AABoxKDTree2dNode对象的left_subnode_、right_subnode_成员（这两个成员也是AABoxKDTree2dNode对象）。
6.	对vector中的所有对象调用InitObjects函数进行处理。

InitObjects函数说明：
原型：void InitObjects(const std::vector<ObjectPtr> &objects)
输入：包含对象指针的vector
1.	如果当前分割轴为x轴，则把vector中的所有对象按照成员min_x的值从小到大排列存到objects_sorted_by_min_中，把vector中的所有对象按照成员max_x的值从大到小排列存到objects_sorted_by_max_中。
2.	把步骤1中用于排序的min_x值存到objects_sorted_by_min_bound_中，用于排序的max_x值存到objects_sorted_by_max_bound_中。
3.	如果步骤1、2中的分割轴为y轴，则操作类似。

AABoxKDTree2dNode的查找方法（1）
原型：ObjectPtr GetNearestObject(const Vec2d &point) const
输入：点坐标point
实际调用了GetNearestObjectInternal(point, &min_distance_sqr, &nearest_object)，其中min_distance_sqr设置为了无穷大

AABoxKDTree2dNode的查找方法（2）
原型：void GetNearestObjectInternal(const Vec2d &point,
                                double *const min_distance_sqr,
                                ObjectPtr *const nearest_object) const
功能：查找kdtree中距离指定点最近的对象。
1.	计算point到当前查找用到的平面矩形的最短距离（point在平面矩形内时距离为0）。如果该距离大于等于min_distance_sqr 与ε（念epsilon）的差值，则结束查找并返回。
2.	如果当前分割轴为x轴方向，则取point的x坐标与分割轴进行比较，如果x坐标小于分割轴，则在当前AABoxKDTree2dNode对象的左子KDtree中递归调用本函数进行查找；否则，在右子KDtree中递归进行查找。（如果当前分割轴为y轴方向，操作类似）。
3.	如果查找后const min_distance_sqr被更新为一个小于等于ε的值，则查找结束并返回。
4.	如果先查找的是左子KDtree，则依次计算objects_sorted_by_min_中各对象（已经按min_x从小到大排序）到point的x坐标的距离，如果该距离小于min_distance_sqr，则更新min_distance_sqr和nearest_object。当xxx时退出计算。
5.	如果查找后const min_distance_sqr被更新为一个小于等于ε的值，则查找结束并返回。
6.	此时尚未返回的话，那么：如果前面查找的是左子KDtree，则再对右子Kdtree进行查找；如果前面查找的是右子KDtree，则再对左子Kdtree进行查找。
