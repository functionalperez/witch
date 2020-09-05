#include <cstdio>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <stdexcept>
#include <yaml-cpp/yaml.h>

using namespace fmt::literals;

template <class T>
concept stringable = requires(T const& t) {
    {t.string()};
};

template <stringable S>
struct fmt::formatter<S> {
    constexpr auto parse(fmt::format_parse_context& context) {
        auto it = context.begin();
        auto end = context.end();
        if (it == end || *it != '}')
            throw fmt::format_error("invalid format");
        return ++it;
    }

    template <class Context>
    auto format(const S& s, Context& context) {
        return fmt::format_to(context.out(), s.string());
    }
};

namespace fs = std::filesystem;
namespace yaml = YAML;
yaml::Node loadConfig(fs::path const& p) {
    if (fs::is_directory(p)) {
        if (fs::exists(p / "witch.yaml"))
            return yaml::LoadFile(p / "witch.yaml");
        if (fs::exists(p / "build.yaml"))
            return yaml::LoadFile(p / "build.yaml");

        throw std::runtime_error(
            "loadConfig: couldn't find build file in {}"_format(p));
    } else {
        return yaml::LoadFile(p);
    }
}

int main(int argc, char** argv) {
    try {
        auto file = loadConfig(".");
    } catch (std::exception& ex) {
        fmt::print(stderr,
                   "witch threw an exception.\n"
                   "\n"
                   "Error: {}\n",
                   ex.what());
    }
}
