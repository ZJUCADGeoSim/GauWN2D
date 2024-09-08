module;
#include <CLI/CLI.hpp>


export module zjucad.gauwn_cli.value;

import zjucad.gauwn_cli.cli_subcmd;

namespace zjucad::gauwn_cli {
export Subcmd cmd_value(CLI::App *app, SubcmdContext *ctx);
}
