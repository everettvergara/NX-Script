#pragma once

#include <string>
#include <vector>
#include <optional>

namespace eg
{
    class param_script
    {
    private:
        std::vector<std::string>    param_scripts_;
        const size_t                max_param_stack_;
    
    public:
        param_script(const size_t max)
            : max_param_stack_(max)
        {
            param_scripts_.reserve(max_param_stack_);
        }

        auto size() const -> size_t
        {
            return param_scripts_.size();
        }

        auto will_overflow() -> bool
        {
            return size() == max_param_stack_;
        }

        auto emplace_back(std::string &&str) 
                -> std::optional<const std::string *>
        {
            if (size() == max_param_stack_) return {};
            const auto r = &param_scripts_.emplace_back(std::move(str));
            return r;
        }
    };
}