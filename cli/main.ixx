module;
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>

export module zjucad.gauwn_cli.main;
import zjucad.gauwn_cli.value;
import zjucad.gauwn_cli.spgrad;
import zjucad.gauwn_cli.veclocgrad;
import zjucad.gauwn_cli.cli_subcmd;

namespace zjucad::gauwn_cli {
export int main(int argc, char **argv) {
    spdlog::set_pattern("[%^%l%$] %v");
    spdlog::cfg::load_env_levels();
    CLI::App app{"gauwn"};
    app.require_subcommand(1, 0);
    app.set_config("--config");
    app.option_defaults()->always_capture_default();
    SubcmdContext ctx;
    ctx.subcmd(app.add_subcommand("value"), cmd_value);
    ctx.subcmd(app.add_subcommand("spgrad"), cmd_spgrad);
    ctx.subcmd(app.add_subcommand("veclocgrad"), cmd_veclocgrad);

    try {
        app.parse(argc, argv);
        ctx.exec();

    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    return 0;
}
} // namespace zjucad::gauwn_cli