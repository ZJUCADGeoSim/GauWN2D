module;

#include <coroutine>
#include <vector>
#include <memory>

#include <CLI/CLI.hpp>

export module zjucad.gauwn_cli.cli_subcmd;
import zjucad.gauwn.utils.resumable;

export namespace std {
using std::coroutine_handle;
using std::coroutine_traits;
} // namespace std

namespace zjucad::gauwn_cli {
export using Subcmd = zjucad::gauwn::utils::Resumable;

export constexpr auto dispatch_cmd = std::suspend_always{};

export struct SubcmdContext {
    std::vector<std::unique_ptr<Subcmd>> cmds;
    std::vector<Subcmd *> pending;

    void subcmd(CLI::App *app, auto &&f) {
        auto &ret = cmds.emplace_back(new Subcmd(f(app, this)));
        auto retraw = ret.get();
        app->parse_complete_callback([&, retraw] { pending.push_back(retraw); });
    }

    void exec() {
        for (int i = pending.size() - 1; i >= 0; i--) {
            pending[i]->resume();
        }
        for (int i = 0; i < pending.size(); i++) {
            pending[i]->resume();
        }
    }
};

} // namespace zjucad::gauwn_cli