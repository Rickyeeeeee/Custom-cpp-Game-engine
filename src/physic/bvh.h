#pragma once

#include "core/pch.h"
#include "core/GLM.h"

template<class BoundingVolumeClass, class T>
struct BoundingVolumeNode
{
    enum Type {
        LEAF, NODE
    };
    BoundingVolumeNode() = default;
    BoundingVolumeNode(T* ptr)
        : type{LEAF}, object{ptr}, boundingVolume{ptr->GetBoundingVolume()} {}
    BoundingVolumeNode(BoundingVolumeNode* node_1, BoundingVolumeNode* node_2)
        : type(NODE), Children({ node_1, node_2 }) {}

    Type type; 

    union {
        T* object;
        BoundingVolumeNode* Children[2];
    }
    int numObjects;
    BoundingVolumeClass boundingVolume;
};

template<class BoundingVolumeClass, class T>
class BoundingVolumeTreeIterator
{
public:

private:
    BoundingVolumeNode<BoundingVolumeClass, T>* m_pointer;
};


template<class BoundingVolumeClass, class T>
class BoundingVolumeHierachy
{
private:
    using Iterator              = BoundingVolumeTreeIterator<BoundingVolumeClass, T>;
    using Node                  = BoundingVolumeNode<BoundingVolumeClass, T>;

    using PartitionList         = std::vector<T*>;
    using PartitionListIterator = PartitionList::iterator;

public:
    BoundingVolumeHierachy(const std::vector<T*>& objects_array, int numObjects);
    void Update();
    void Destroy();
private:
    void TopDownConstruct(Node* tree, PartitionListIterator itr, int numObjects);
    int  PartitionObjects(PartitionListIterator itr, int numObjects);
private:
    Node*           m_RootNode;
    Node*           m_NodePool;
    PartitionList   m_PartitionList;

    int             m_NodePoolIndex;
    int             m_NodeCapacity;
    int             m_NumObjects;
    int             m_SortAxis;
};

template<class BoundingVolumeClass, class T>
BoundingVolumeHierachy::BoundingVolumeHierachy(const std::vector<T*>& objects_array, int numObjects)
{
    m_NodeCapacity  = objects_array.size() * objects_array.size();
    m_NodePool      = new Node[m_NodeCapacity];
    m_NodePoolIndex = 0;
    m_NumObjects    = numObjects;
    for (int i = 0; i < m_NumObjects; i++)
        m_PartitionList.push_back(objects_array[i]);
    TopDownConstruct(&m_RootNode, m_PartitionList.begin(), m_NumObjects);
}

template<class BoundingVolumeClass, class T, class T_Itr>
void BoundingVolumeHierachy::Update()
{
    for (auto& [data, vector]  : m_PartitionList)
        vector = data->GetCenter();
}

template<class BoundingVolumeClass, class T>
int  BoundingVolumeHierachy::PartitionObjects(PartitionListIterator itr, int numObjects)
{
    std::sort(itr, itr + numObjects, [](T* object_1, T* object_2){
        return object_1.GetCenter()[m_SortAxis] < object_2.GetCenter()[m_SortAxis];
    });

    if (m_SortAxis >= 2)
        m_SortAxis = 0;
    else 
        m_SortAxis++;
    return numObjects / 2;
}

template<class BoundingVolumeClass, class T>
void TopDownConstruct(Node* tree, PartitionListIterator itr, int numObjects)
{
    auto pNode = new Node;
    *tree = pNode;
    pNode->boundingVolume = BoundingVolumeClass::ComputeBoundingVolume(itr, numObjects);
    if (numObjects <= 1)
    {
        pNode->type = LEAF;
        pNode->numObjects = numObjects;
        pNode->object = &object[0];
    }
    else
    {
        pNode->type = NODE;
        int k = PartitionObjects(itr, numObjects);
        TopDownConstruct(&(pNode->left), &object[0], k);
        TopDownConstruct(&(pNode->right), &object[k], numObjects - k);
    }
}
