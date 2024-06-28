// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "sgl/core/fwd.h"
#include "sgl/core/stream.h"
#include "sgl/core/enum.h"
#include "sgl/core/platform.h"

#include <map>
#include <functional>
#include <filesystem>
#include <chrono>

namespace sgl {

/// Types of file system event that can be reported.
enum class FileSystemWatcherChange {
    invalid,
    added,
    removed,
    modified,
    renamed,
};
SGL_ENUM_INFO(
    FileSystemWatcherChange,
    {
        {FileSystemWatcherChange::invalid, "invalid"},
        {FileSystemWatcherChange::added, "added"},
        {FileSystemWatcherChange::removed, "removed"},
        {FileSystemWatcherChange::modified, "modified"},
        {FileSystemWatcherChange::renamed, "renamed"},
    }
);
SGL_ENUM_REGISTER(FileSystemWatcherChange);

/// Init options for FileSystemWatcher.
struct FileSystemWatchDesc {

    /// Directory to monitor.
    std::filesystem::path directory;

    /// Whether to monitor subdirectories.
    bool recursive{true};
};

/// Data reported on a given file system event to a file monitored
/// by FileSystemWatcher.
struct FileSystemWatchEvent {

    /// Path of file that has changed.
    std::filesystem::path path;

    /// Change type.
    FileSystemWatcherChange change;

    /// System time change was recorded.
    std::chrono::system_clock::time_point time;
};

/// Monitors directories for changes and calls a callback when they're detected.
/// The watcher automatically queues up changes until disk has been idle for
/// a period. Relies on regular polling of update().
class SGL_API FileSystemWatcher : public Object {
    SGL_OBJECT(FileSystemWatcher)
public:
    using ChangeCallback = std::function<void(std::span<FileSystemWatchEvent>)>;

    FileSystemWatcher();
    ~FileSystemWatcher();

    /// Add watch of a new directory.
    void add_watch(const FileSystemWatchDesc& desc);

    /// Remove existing watch.
    void remove_watch(const std::filesystem::path& directory);

    /// Get number of active watches
    size_t get_watch_count() const { return m_watches.size(); }

    /// Get callback for file system events.
    ChangeCallback get_on_change() const { return m_on_change; }

    /// Set callback for file system events.
    void set_on_change(ChangeCallback on_change);

    /// Update function to poll the watcher + report events.
    void update();

    /// Internal function called when OS reports an event.
    void _notify_change(const std::filesystem::path& path, FileSystemWatcherChange change);

private:
    /// Next unique id to be assigned to a given watch.
    uint32_t m_next_id{1};

    /// Map of id->watch.
    std::map<int, FileSystemWatchState*> m_watches;

    /// Watch event callback.
    ChangeCallback m_on_change;

    /// Events reported since last call to watch event callback.
    std::vector<FileSystemWatchEvent> m_queued_events;

    /// Time last event was recorded.
    std::chrono::system_clock::time_point m_last_event;

    /// Get watch by id.
    FileSystemWatchState* get_watch(uint32_t id);

    /// Releases OS monitoring for a given watch.
    void stop_watch(FileSystemWatchState* state);
};


} // namespace sgl
