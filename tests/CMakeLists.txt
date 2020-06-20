add_executable(editiontree_test editiontree.cpp)
add_test(NAME editiontree COMMAND editiontree_test)

target_link_libraries(editiontree_test
    Symbolibre::Symbolibre ${QT_DEPS} Qt5::Test)
