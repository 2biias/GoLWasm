#if !defined(WATERMARKRESOURCE_HPP)
#define WATERMARKRESOURCE_HPP

#include <experimental/memory_resource>
#include <iostream>

namespace Utilities::Resource
{

class WatermarkResource : public std::experimental::pmr::memory_resource
{
public:
    WatermarkResource(std::experimental::pmr::memory_resource* upstream = std::experimental::pmr::get_default_resource())
    : upstream_(upstream)
    {}

    void printWatermark() {
        std::cout << "High watermark: " << waterMark_ << "bytes, allocated now: " << bytesAllocated_ << "bytes" << std::endl;
    }

private:
    void *do_allocate(size_t bytes, size_t align) override {
        bytesAllocated_ += bytes;
        waterMark_ = (bytesAllocated_ > waterMark_) ? bytesAllocated_ : waterMark_;
        return upstream_->allocate(bytes, align);
    }

    void do_deallocate(void *p, size_t bytes, size_t align) override {
        bytesAllocated_ -= bytes;
        upstream_->deallocate(p, bytes, align);
    }

    bool do_is_equal(std::experimental::pmr::memory_resource const &other) const noexcept override {
        return upstream_->is_equal(other);
    }

    int waterMark_ = 0;
    int bytesAllocated_ = 0;
    std::experimental::pmr::memory_resource* upstream_;
};

}

#endif // WATERMARKRESOURCE_HPP