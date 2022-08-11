#include "BV.h"

BoundingSphere::BoundingSphere(float r, const Vector3& p)
{
    
}

bool BoundingSphere::Intersect(const BoundingSphere& other)
{
    auto R = (radius + other.radius);
    auto Dis = position - other.position;
    return R * R > (Dis.x * Dis.x + Dis.y * Dis.y + Dis.z * Dis.z);
}

void BVH::TopDownConstruct()
{   
    rTopDownTree(&m_Root, 0, m_Objects.size());
}

void BVH::BottomUpConstruct()
{
    
}

void BVH::Add(const BV& bv)
{
    m_Objects.push_back(bv);
}

void BVH::rTopDownTree(Node** tree, uint32_t start, uint32_t end)
{
    const size_t MINIMUM_OBJECTS_SIZE = 1;
    auto size = end - start;
    Node* pNode = new Node;
    *tree = pNode;
    pNode->bv = BValgorithm::ComputeBoundingVolume(m_Objects, start, end);

    if (size <= MINIMUM_OBJECTS_SIZE)
    {
        pNode->type = Node::Type::LEAF;
        pNode->numObjects = m_Objects.size();
        pNode->object = 0;
    }
    else
    {
        pNode->type = Node::Type::NONE;

        static int axis = 0;

        uint32_t k = BValgorithm::PartitionObjects(m_Objects, start, end, [&](aabb& a, aabb& b){
            return ((a.Max + a.Min) / 2.0f)[axis] < ((b.Max + b.Min) / 2.0f)[axis];
        });

        axis = (axis + 1) % 3;

        rTopDownTree(&(pNode->left), start, k);
        rTopDownTree(&(pNode->right), k, end);
    }
    
}


BoundingSphere BValgorithm::ComputeBoundingVolume(const std::vector<BoundingSphere>& objects, uint32_t start, uint32_t end)
{
}

uint32_t BValgorithm::PartitionObjects(std::vector<BoundingSphere>& objects, uint32_t start, uint32_t end)
{
    
}

aabb BValgorithm::ComputeBoundingVolume(const std::vector<aabb>& objects, uint32_t start, uint32_t end)
{
    assert(end > start);
    const auto& first = objects[start];
    Vector3 maxPos = first.Max;
    Vector3 minPos = first.Min;
    for (auto i = start + 1; i < end; i++)
    {
        const auto& object = objects[i];
        maxPos.x = std::max(maxPos.x, object.Max.x);
        minPos.x = std::min(minPos.x, object.Min.x);
        maxPos.y = std::max(maxPos.y, object.Max.y);
        minPos.y = std::min(minPos.y, object.Min.y);
        maxPos.z = std::max(maxPos.z, object.Max.z);
        minPos.z = std::min(minPos.z, object.Min.z);
    }
    return aabb {
        maxPos, minPos
    };
}

template <typename ComprFunc>
uint32_t BValgorithm::PartitionObjects(std::vector<aabb>& objects, uint32_t start, uint32_t end, ComprFunc func)
{
    std::sort(objects.begin() + start, objects.begin() + end, func);
    return (end + start) / 2;
}
