set(editiontree_SOURCES
    EditionArea.cpp
    Flow.cpp
    FontResizer.cpp
    Frac.cpp
    fromGiacGen.cpp
    InternalEditionNode.cpp
    Operator.cpp
    Paren.cpp
    Power.cpp
    Root.cpp
    serialization.cpp
    Sigma.cpp
    Variable.cpp
)
set(editiontree_HEADERS
    ActiveEditionNodeVisitor.hpp
    EditionArea.hpp
    EditionNode.hpp
    EditionTree.hpp
    Flow.hpp
    Frac.hpp
    graphicalParameters.hpp
    InternalEditionNode.hpp
    Operator.hpp
    Paren.hpp
    Power.hpp
    Root.hpp
    serialization.hpp
    Sigma.hpp
    Variable.hpp
    visitors.hpp
)

add_library(editiontree OBJECT ${editiontree_SOURCES})
target_link_libraries(editiontree
    Giac::Giac
    ${QT_DEPS}
)

install(
    FILES ${editiontree_HEADERS}
    DESTINATION ${SL_INCLUDE_INSTALL_DIR}/editiontree
)
