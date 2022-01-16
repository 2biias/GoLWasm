#if !defined(CELLMANAGER_HPP)
#define CELLMANAGER_HPP

#include <map>
#include <chrono>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <experimental/memory_resource>

#include <gameEngine/gameComponent.hpp>
#include <utilities/math/vector2d.hpp>
#include <utilities/resource/watermarkResource.hpp>
#include <gameOfLife/components/cell.hpp>
#include <gameOfLife/components/common.hpp>
#include <gameOfLife/components/algorithm/cellAlgo.hpp>

namespace GameOfLife::Components
{

using namespace Utilities;
using namespace std::chrono_literals;

template<typename Engine>
class CellManager : public GameEngine::GameComponent, public std::enable_shared_from_this<CellManager<Engine>>
{
public:
    using PosToCellStateMap = std::map<std::pair<int,int>, CellState>;
    using PosToCell = std::map<std::pair<int,int>, std::shared_ptr<Cell<Engine>>>;
    using CellAllocator = std::experimental::pmr::polymorphic_allocator<std::byte>;

    CellManager(Engine& engine, const uint32_t cellsPrAxis, const uint32_t cellSize) 
    :   engine_(engine),
        cellSize_(cellSize),
        cellsPrAxis_(cellsPrAxis),
        numberOfCells_(cellsPrAxis*cellsPrAxis) //cellResource_(&pool_) ,pool_(internal_buffer_.data(), internal_buffer_.size())
    {
        initializeCells();
    }

    void registerInEngine() {
        id_ = engine_.registerComponent(this->shared_from_this());
    }

    void update() {
        if(started_ == true) {
            if(tickCount_++ >= 1) {

                auto start = std::chrono::high_resolution_clock::now();
                cellAlgo_.doParallel(currentGen_, nextGen_);
                //cellAlgo_.doSingle(currentGen_, nextGen_);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                //std::cout << "duration: " << duration << "us" << std::endl;

                updateCellObjects();

                currentGen_.clear();
                currentGen_.swap(nextGen_);

                tickCount_ = 0;
                genNumber_++;
            }
        }
    }

    void render() {
        auto renderer_ptr = engine_.getRenderer();
        const std::string text = "Cells: " + std::to_string(posToCell_.size());
        const std::string genText = "Gen: " + std::to_string(genNumber_);

        renderer_ptr->drawText(Math::Vector2D(0,cellsPrAxis_*cellSize_+50), Math::Vector2D((cellsPrAxis_/2)*cellSize_, cellsPrAxis_*cellSize_+100), Color(0xff, 0xfa, 0xfa), text);
        renderer_ptr->drawText(Math::Vector2D((cellsPrAxis_/2)*cellSize_+10,cellsPrAxis_*cellSize_+50),
                                Math::Vector2D(cellsPrAxis_*cellSize_-10,cellsPrAxis_*cellSize_+100),
                                Color(0xff, 0xfa, 0xfa), genText);

    }

    void updateCellObjects(){
        
        for(auto& [pos, state] : currentGen_) {
            if(nextGen_[pos] != state)  {
                if(nextGen_[pos] == CellState::alive) {
                    //posToCell_.insert(std::make_pair(pos, std::allocate_shared<Cell<Engine>, CellAllocator>(&cellResource_, engine_, Math::Vector2D(pos), cellSize_)));
                    posToCell_.insert(std::make_pair(pos, std::make_shared<Cell<Engine>>(engine_, Math::Vector2D(pos), cellSize_)));
                    posToCell_[pos]->registerInEngine();
                } else {
                    posToCell_[pos]->kill();
                    posToCell_[pos] = nullptr;
                    posToCell_.erase(pos);
                }
            }
        }
    }

    void initializeCells() {
        for(size_t x = 0; x < cellsPrAxis_; ++x) {
            for(size_t y = 0; y < cellsPrAxis_; ++y) {
                currentGen_[std::make_pair(x, y)] = CellState::dead;
            }
        }
    }

    void click(const Math::Vector2D pos) {
        
        int cellNumX = pos.getX() / cellSize_;
        int cellNumY = pos.getY() / cellSize_;
        auto cellPos = std::make_pair(cellNumX, cellNumY);
        if(currentGen_[cellPos] == CellState::dead) {
            currentGen_[cellPos] = CellState::alive;
            //posToCell_.insert(std::make_pair(cellPos, std::allocate_shared<Cell<Engine>, CellAllocator>(&cellResource_, engine_, Math::Vector2D(cellPos), cellSize_)));
            posToCell_.insert(std::make_pair(cellPos, std::make_shared<Cell<Engine>>(engine_, Math::Vector2D(cellPos), cellSize_)));
            posToCell_[cellPos]->registerInEngine();
        } else {                                
            currentGen_[cellPos] = CellState::dead;
            posToCell_[cellPos]->kill();
            posToCell_[cellPos] = nullptr;
            posToCell_.erase(cellPos);
        }
    }

    std::pair<Math::Vector2D, Math::Vector2D> getHitBox() const {
        return std::make_pair(Math::Vector2D(0,0), Math::Vector2D(cellsPrAxis_*cellSize_, cellsPrAxis_*cellSize_));
    }

    void startPause() { 
        started_ = !started_;
        //cellResource_.printWatermark();
    }

    void resetCells() {
        genNumber_ = 0;
        for(auto& [pos, cell] : posToCell_) {
            cell->kill();
            cell = nullptr;
        }
        posToCell_.clear();
        initializeCells();
    }

private:
    Engine& engine_;
    uint32_t numberOfCells_;
    uint32_t cellsPrAxis_;
    uint32_t cellSize_;
    uint32_t id_;
    uint32_t tickCount_ = 0;
    uint32_t genNumber_ = 0;
    bool started_ = false;

    std::array<std::byte, CELLSPRAXIS*CELLSPRAXIS*96> internal_buffer_;
    //std::pmr::monotonic_buffer_resource pool_;
    //Resource::WatermarkResource cellResource_;

    Algorithm::GameOfLifeAlgorithm cellAlgo_;
    PosToCellStateMap currentGen_;
    PosToCellStateMap nextGen_;
    PosToCell posToCell_;
};

template <typename Engine>
using CellManagerTypeList = typename Meta::Append<COMPONENTS_LIST<Engine>, CellManager<Engine>>::type;
#undef COMPONENTS_LIST
#define COMPONENTS_LIST CellManagerTypeList

}

#endif // CELLMANAGER_H