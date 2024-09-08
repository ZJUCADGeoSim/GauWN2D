module;

#include <CLI/CLI.hpp>


export module zjucad.gauwn_cli.veclocgrad;


import zjucad.gauwn_cli.cli_subcmd;


namespace zjucad::gauwn_cli {
export Subcmd cmd_veclocgrad(CLI::App *app, SubcmdContext *ctx);
} // namespace zjucad::gauwn_cli