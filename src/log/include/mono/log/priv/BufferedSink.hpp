#pragma once

#include <spdlog/details/log_msg_buffer.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

namespace mono::log::priv
{

class BufferedSink final : public spdlog::sinks::base_sink<std::mutex>
{
    public:
    const std::vector<spdlog::details::log_msg_buffer>& buffer() const { return m_buffer; }

    void clear() { m_buffer.clear(); }

    private:
    void sink_it_(const spdlog::details::log_msg& msg) final { m_buffer.emplace_back(msg); }

    void flush_() final
    {
        // Do nothing, as this is a buffered sink.
    }

    private:
    std::vector<spdlog::details::log_msg_buffer> m_buffer{};
};

}  // namespace mono::log::priv
