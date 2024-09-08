module;
#include <unordered_map>

export module zjucad.gauwn.utils.union_find;

namespace zjucad::gauwn::utils {
export template <typename Id = int, typename Set = std::unordered_map<Id, Id>>
Id ufind(const Id &a, Set &parent) {
    if (parent[a] == a)
        return a;
    parent[a] = ufind(parent[a], parent);
    return parent[a];
};
export template <typename Id = int, typename Set = std::unordered_map<Id, Id>>
void uadd(const Id &a, const Id &b, Set &parent) {
    parent[ufind(a, parent)] = ufind(b, parent);
}

export template <typename Id = int, typename Set = std::unordered_map<Id, Id>>
bool usame(const Id &a, const Id &b, Set &parent) {
    return ufind(a, parent) == ufind(b, parent);
}
} // namespace zjucad::gauwn::utils