#pragma once

#include "fwd.h"
#include "stage_object_id.h"

#include "sgl/core/macros.h"

#include <atomic>
#include <mutex>

namespace sgl::render {

template<StageObjectKind kind, typename T, size_t PageSize = 1024, size_t PageCount = 1024>
class Pool {
public:
    SGL_NON_COPYABLE_AND_MOVABLE(Pool);

    using ID = ObjectID<kind>;

    Pool()
    {
        for (auto& page : m_pages)
            page = nullptr;
    }

    ~Pool()
    {
        for (auto& page : m_pages)
            delete page;
    }

    T* get(ID id)
    {
        uint64_t index = id.index();
        uint64_t page_index = index / PageSize;
        uint64_t object_index = index % PageSize;
        Page* page = m_pages[page_index].load();
        SGL_ASSERT(page_index < PageCount);
        return page->used[object_index] ? &page->objects[object_index] : nullptr;
    }

    const T* get(ID id) const
    {
        uint64_t index = id.index();
        uint64_t page_index = index / PageSize;
        uint64_t object_index = index % PageSize;
        Page* page = m_pages[page_index].load();
        SGL_ASSERT(page_index < PageCount);
        return page->used[object_index] ? &page->objects[object_index] : nullptr;
    }

    ID create()
    {
        size_t page_index = 0;
        while (page_index < PageCount) {
            Page* page = m_pages[page_index].load();
            if (page) {
                uint32_t free_count = page->free_count.load();
                if (free_count == 0) {
                    ++page_index;
                    continue;
                }
            } else {
                Page* new_page = new Page();
                if (m_pages[page_index].compare_exchange_strong(page, new_page)) {
                    page = new_page;
                } else {
                    delete new_page;
                    continue;
                }
            }
            SGL_ASSERT(page);
            for (uint32_t object_index = 0; object_index < PageSize; ++object_index) {
                uint32_t used = page->used[object_index].load();
                if (used)
                    continue;
                if (page->used[object_index].compare_exchange_strong(used, 1)) {
                    page->free_count.fetch_sub(1);
                    return ID{page_index * PageSize + object_index};
                }
            }
            ++page_index;
        }
        SGL_THROW("Pool is full");
    }

    void destroy(ID id)
    {
        uint64_t index = id.index();
        uint64_t page_index = index / PageSize;
        uint64_t object_index = index % PageSize;
        Page* page = m_pages[page_index].load();
        SGL_ASSERT(page_index < PageCount && page->used[object_index]);
        page->used[object_index].store(0);
        page->free_count.fetch_add(1);
    }

private:
    void allocate_page() { m_pages.emplace_back(new Page()); }

    struct Page {
        T objects[PageSize];
        std::atomic<uint32_t> used[PageSize];
        // bool free[PageSize];
        std::atomic<uint32_t> free_count{PageSize};
        // std::mutex mutex;
    };

    std::atomic<Page*> m_pages[PageCount];
};

} // namespace sgl::render
