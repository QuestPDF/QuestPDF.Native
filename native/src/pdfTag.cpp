#include <iostream>

#include "export.h"

#include <memory>
#include <vector>
#include "include/core/SkCanvas.h"
#include "include/docs/SkPDFDocument.h"

extern "C" {

QUEST_API SkPDF::StructureElementNode* pdf_structure_element_create(int nodeId, char* type, char* alt, char* lang) {
    auto element = new SkPDF::StructureElementNode();
    element->fNodeId = nodeId;

    element->fTypeString = type;
    element->fAlt = alt;
    element->fLang = lang;

    return element;
}

QUEST_API void pdf_structure_element_set_children(SkPDF::StructureElementNode* parent, SkPDF::StructureElementNode** children, int childCount) {
    parent->fChildVector = std::vector<std::unique_ptr<SkPDF::StructureElementNode>>(children, children + childCount);
}

QUEST_API void pdf_structure_element_add_attribute_integer(SkPDF::StructureElementNode* node, const char* owner, const char* name, const int value) {
    node->fAttributes.appendInt(owner, name, value);
}

QUEST_API void pdf_structure_element_delete(const SkPDF::StructureElementNode* element) {
    delete element;
}

}
