#pragma once
#include <vector>

template <typename T>
class Iterator {
public:
    virtual T next() = 0;
    virtual T peek() = 0;
    virtual void reset() = 0;
    virtual bool hasNext() = 0;
    virtual ~Iterator() = default;
};

namespace bot {

    class ITileNode;

    class TileNodeIterator : public Iterator<ITileNode*> {
    private:
        std::vector<ITileNode*>* _nodes;
        int _index = 0;
    public:
        TileNodeIterator(std::vector<ITileNode*>* nodes);
        ~TileNodeIterator() override;

        ITileNode* next() override;
        ITileNode* peek() override;
        void reset() override;
        bool hasNext() override;
    };

}
