#include "element.h"

#include <stdexcept>
#include <unordered_map>

const ElementStyle& element_style(const std::string& symbol)
{
    static const std::unordered_map<std::string, ElementStyle> table = {
        { "H", { 0.30f, glm::vec3(0.95f, 0.95f, 0.95f) } },
        { "C", { 0.60f, glm::vec3(0.30f, 0.30f, 0.30f) } },
        { "N", { 0.6f, glm::vec3(0.30f, 0.30f, 0.95f) } },
        { "O", { 0.6f, glm::vec3(0.95f, 0.35f, 0.35f) } },
    };

    auto it = table.find(symbol);
    if (it == table.end()) {
        throw std::runtime_error("Unsupported element: " + symbol);
    }
    return it->second;
}
