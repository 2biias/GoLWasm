#if !defined(ALGORITHM_HPP)
#define ALGORITHM_HPP

#include <vector>
#include <map>
#include <mutex>
#include <future>
#include <algorithm>
#include <gameOfLife/components/common.hpp>

namespace GameOfLife::Components::Algorithm
{

template <typename T,typename U>                                                   
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first+r.first,l.second+r.second};                                    
}   

class GameOfLifeAlgorithm
{
public:
    using PosToCellStateMap = std::map<std::pair<int,int>, CellState>;

    void doSingle(PosToCellStateMap& currentGen, PosToCellStateMap& nextGen) {
        
        std::transform(currentGen.begin(), currentGen.end(), std::inserter(nextGen, nextGen.end()),
                [&](PosToCellStateMap::value_type& posToCellState) -> PosToCellStateMap::value_type {
                    int aliveNeighbors = countAliveNeigbors(currentGen, posToCellState.first);
                    PosToCellStateMap::mapped_type newState = checkRules(aliveNeighbors, posToCellState.second);
                    return std::make_pair(posToCellState.first, newState);
            });
    }

    
    void doParallel(const PosToCellStateMap& currentGen, PosToCellStateMap& nextGen) {

        using algorithm_futures = std::vector<std::future<PosToCellStateMap>>;
        
        uint32_t numThreads = 4;
        uint32_t cellsPrThread = (CELLSPRAXIS*CELLSPRAXIS) / numThreads;
        algorithm_futures future_container(numThreads);
        PosToCellStateMap::const_iterator sliceBegin = currentGen.cbegin();

        for(auto& future : future_container)
        {
            future = std::async(std::launch::async, [&, sliceBegin] {
                
                PosToCellStateMap nextSlice;
                std::transform(sliceBegin, std::next(sliceBegin, cellsPrThread), std::inserter(nextSlice, nextSlice.end()),
                [&](const PosToCellStateMap::value_type& posToCellState) -> PosToCellStateMap::value_type {
                    int aliveNeighbors = countAliveNeigbors(currentGen, posToCellState.first);
                    PosToCellStateMap::mapped_type newState = checkRules(aliveNeighbors, posToCellState.second);
                    return std::make_pair(posToCellState.first, newState);
                });
                return nextSlice;
            });
            
            sliceBegin = std::next(sliceBegin, cellsPrThread);
        }
        for (auto& future : future_container) {
            future.wait();
            PosToCellStateMap nextSlice = future.get();
            nextGen.insert(nextSlice.begin(), nextSlice.end());
        }
    }

    int countAliveNeigbors(const PosToCellStateMap& currentGen, const PosToCellStateMap::key_type pos) const {
        int aliveNeighbors = std::count_if(neighbors_.begin(), neighbors_.end(),
                [&](PosToCellStateMap::key_type relativePosition) {
                        PosToCellStateMap::key_type neighborPos = relativePosition + pos;
                        if(currentGen.count(neighborPos) > 0) {
                            return ((currentGen.find(neighborPos))->second == CellState::alive) ? true : false;
                        }
                        return false;
                    });
        return aliveNeighbors;
    }

    PosToCellStateMap::mapped_type checkRules(int aliveNeighbors, PosToCellStateMap::mapped_type cellState) const {
        if((cellState == CellState::alive) && (aliveNeighbors == 2 || aliveNeighbors == 3))
            return CellState::alive;
        else if((cellState == CellState::dead) && (aliveNeighbors == 3))
            return CellState::alive;
        else
            return CellState::dead;
    }

private:
    const std::vector<PosToCellStateMap::key_type> neighbors_ = {
            std::make_pair(0,1),
            std::make_pair(1,0),
            std::make_pair(1,1),
            std::make_pair(0,-1),
            std::make_pair(-1,0),
            std::make_pair(-1,-1),
            std::make_pair(-1,1),
            std::make_pair(1,-1)
    };
};

}

#endif // ALGORITHM_HPP
