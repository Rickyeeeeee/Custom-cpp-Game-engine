#pragma once
#include <vector>
#include <Engine.h>

struct BVbase
{
    virtual Vector3 GetCenter() = 0;
};

struct BoundingSphere : public BVbase
{
    virtual Vector3 GetCenter() override
    {
        return position;
    }

    BoundingSphere() = default;
    BoundingSphere(float r, const Vector3& p);
    bool Intersect(const BoundingSphere& other);

    float radius;
    Vector3 position;
};

struct aabb : public BVbase
{
    aabb(const Vector3& max, const Vector3& min)
        : Max(max), Min(min) {}
    aabb() = default;
    Vector3 Max;
    Vector3 Min;
    virtual Vector3 GetCenter() override
    {
        return (Max + Min) / 2.0f;
    }
};

struct BValgorithm
{
    static BoundingSphere ComputeBoundingVolume(const std::vector<BoundingSphere>& objects, uint32_t start, uint32_t end);
    static uint32_t PartitionObjects(std::vector<BoundingSphere>& objects, uint32_t start, uint32_t end);
    static aabb ComputeBoundingVolume(const std::vector<aabb>& objects, uint32_t start, uint32_t end);
    template <typename ComprFunc>
    static uint32_t PartitionObjects(std::vector<aabb>& objects, uint32_t start, uint32_t end, ComprFunc func);
};

template <typename BV>
struct BVnode
{
    enum Type {
        NONE, LEAF
    } type;
    operator uint32_t() {
        return object;
    }
    BV bv;
    uint32_t object;
    uint32_t numObjects;
    BVnode* left{ nullptr };
    BVnode* right{ nullptr };
};


class BVH
{
    using BV = aabb;
    using Node = BVnode <aabb>;
public:
    void TopDownConstruct();
    void BottomUpConstruct();
    void Add(const BV& bv);
    const Node* GetRoot()
    {
        return m_Root;
    }
private:
    struct BVobject : public BV
    {
        
    };
private:
    void rTopDownTree(Node** tree, uint32_t start, uint32_t end);
private:
private:
    // std::vector<uint32_t> m_Objects;
    std::vector<BV> m_Objects;
    Node* m_Root{ nullptr };
};