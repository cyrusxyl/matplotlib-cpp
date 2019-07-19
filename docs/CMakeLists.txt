find_package(Doxygen REQUIRED)

set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN NO)

doxygen_add_docs(doxygen ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/README.md COMMENT "Generate HTML Doxygen docs")
