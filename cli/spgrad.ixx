module;

#include <CLI/CLI.hpp>


export module zjucad.gauwn_cli.spgrad;


import zjucad.gauwn_cli.cli_subcmd;


namespace zjucad::gauwn_cli {
export Subcmd cmd_spgrad(CLI::App *app, SubcmdContext *ctx);

} // namespace zjucad::gauwn_cli